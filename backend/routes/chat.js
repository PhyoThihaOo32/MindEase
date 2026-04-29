const express = require("express");
const {
  createSupportiveResponse,
  detectBackendRisk,
  moderateMessage,
  moderationRequiresCrisisResponse
} = require("../services/openai");
const { CRISIS_RESPONSE } = require("../templates/crisisResponse");

const router = express.Router();
const MAX_MESSAGE_CHARS = 1800;
const MAX_HISTORY_TURNS = 12;
const MAX_HISTORY_TEXT_CHARS = 600;
const MAX_SAFETY_CONTEXT_CHARS = 2400;

function sanitizeHistory(rawHistory) {
  if (!Array.isArray(rawHistory)) {
    return [];
  }

  return rawHistory
    .slice(-MAX_HISTORY_TURNS)
    .map((entry) => {
      const role = entry && (entry.role === "assistant" || entry.role === "user")
        ? entry.role
        : "";
      const text = entry && typeof entry.text === "string"
        ? entry.text.replace(/\s+/g, " ").trim()
        : "";

      if (!role || !text) {
        return null;
      }

      return {
        role,
        text: text.slice(0, MAX_HISTORY_TEXT_CHARS)
      };
    })
    .filter(Boolean);
}

function buildSafetyContext(message, history) {
  const userTurns = history
    .filter((entry) => entry.role === "user")
    .map((entry) => entry.text);

  const combined = [...userTurns, message]
    .filter(Boolean)
    .join("\n")
    .trim();

  if (combined.length <= MAX_SAFETY_CONTEXT_CHARS) {
    return combined;
  }

  return combined.slice(combined.length - MAX_SAFETY_CONTEXT_CHARS);
}

router.post("/", async (req, res, next) => {
  try {
    const message = typeof req.body.message === "string" ? req.body.message.trim() : "";
    const history = sanitizeHistory(req.body.history);
    const safetyContext = buildSafetyContext(message, history);

    if (!message) {
      return res.status(400).json({ error: "Message is required." });
    }

    if (message.length > MAX_MESSAGE_CHARS) {
      return res.status(400).json({
        error: `Message is too long. Please keep it under ${MAX_MESSAGE_CHARS} characters.`
      });
    }

    // Safety first: every user message is checked with OpenAI moderation before any chat model call.
    const moderation = await moderateMessage(safetyContext);
    const backendRisk = detectBackendRisk(safetyContext);
    const moderationRisk = moderationRequiresCrisisResponse(moderation);

    if (backendRisk || moderationRisk) {
      return res.json({
        reply: CRISIS_RESPONSE,
        crisis: true,
        source: backendRisk ? "backend-risk-check" : "openai-moderation"
      });
    }

    const reply = await createSupportiveResponse(message, history);

    res.json({
      reply,
      crisis: false
    });
  } catch (error) {
    next(error);
  }
});

module.exports = router;
