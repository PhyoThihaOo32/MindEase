/* ── Falling Leaves ─────────────────────────────────────────────────────── */
(function() {
  const canvas = document.getElementById('leaf-canvas');
  const emojis = ['🍃','🍂','🍀','🌿','🍁','☘️'];
  const count = 18;
  for (let i = 0; i < count; i++) {
    const leaf = document.createElement('div');
    leaf.className = 'leaf';
    leaf.textContent = emojis[Math.floor(Math.random() * emojis.length)];
    leaf.style.cssText = `
      left: ${Math.random() * 100}%;
      font-size: ${14 + Math.random() * 14}px;
      animation-duration: ${8 + Math.random() * 14}s;
      animation-delay: ${-Math.random() * 20}s;
      opacity: ${0.25 + Math.random() * 0.35};
    `;
    canvas.appendChild(leaf);
  }
})();

/* ── Animated counters ──────────────────────────────────────────────────── */
(function() {
  function animateCounter(el, target, duration) {
    let start = null;
    function step(ts) {
      if (!start) start = ts;
      const progress = Math.min((ts - start) / duration, 1);
      el.textContent = Math.round(progress * target);
      if (progress < 1) requestAnimationFrame(step);
    }
    requestAnimationFrame(step);
  }
  const counters = document.querySelectorAll('[data-count]');
  const obs = new IntersectionObserver(entries => {
    entries.forEach(e => {
      if (e.isIntersecting) {
        animateCounter(e.target, parseInt(e.target.dataset.count), 1200);
        obs.unobserve(e.target);
      }
    });
  }, { threshold: 0.5 });
  counters.forEach(c => obs.observe(c));
})();

/* ── Scroll progress bar ────────────────────────────────────────────────── */
const progressBar = document.getElementById('scroll-progress');
window.addEventListener('scroll', () => {
  const h = document.documentElement;
  const pct = (window.scrollY / (h.scrollHeight - h.clientHeight)) * 100;
  progressBar.style.width = pct + '%';
}, { passive: true });

/* ── Back to top ────────────────────────────────────────────────────────── */
const backTop = document.getElementById('back-top');
window.addEventListener('scroll', () => {
  backTop.classList.toggle('show', window.scrollY > 400);
}, { passive: true });

/* ── Dark / Zen Night mode ──────────────────────────────────────────────── */
const darkToggle = document.getElementById('dark-toggle');
let isDark = localStorage.getItem('mindease-dark') === '1';
function applyDark(d) {
  document.body.classList.toggle('dark', d);
  darkToggle.textContent = d ? '☀️ Light' : '🌙 Night';
}
applyDark(isDark);
function toggleDark() {
  isDark = !isDark;
  localStorage.setItem('mindease-dark', isDark ? '1' : '0');
  applyDark(isDark);
}

/* ── Mobile hamburger ───────────────────────────────────────────────────── */
const mobileNav = document.getElementById('mobile-nav');
function toggleMobileNav() { mobileNav.classList.toggle('open'); }
function closeMobileNav() { mobileNav.classList.remove('open'); }

/* ── Nav active link on scroll ──────────────────────────────────────────── */
const sections = document.querySelectorAll('section[id]');
const navLinks = document.querySelectorAll('.nav-links a');

function updateNav() {
  let current = '';
  sections.forEach(sec => {
    if (window.scrollY >= sec.offsetTop - 130) current = sec.id;
  });
  navLinks.forEach(a => {
    a.classList.toggle('active', a.getAttribute('href') === '#' + current);
  });
}
window.addEventListener('scroll', updateNav, { passive: true });
updateNav();

/* ── Scroll-reveal ──────────────────────────────────────────────────────── */
const revealObserver = new IntersectionObserver((entries) => {
  entries.forEach(e => { if (e.isIntersecting) { e.target.classList.add('visible'); } });
}, { threshold: 0.12 });
// Auto-reveal: section headings, grids, cards
document.querySelectorAll(
  '.eyebrow, .section-title, .section-sub, .feature-card, .topic-card, .team-card, .intro-card, .reveal'
).forEach((el, i) => {
  if (!el.classList.contains('reveal')) el.classList.add('reveal');
  // stagger children inside grids
  const parent = el.closest('.features-grid, .topic-grid, .team-cards');
  if (parent) {
    const siblings = parent.querySelectorAll(':scope > *');
    const idx = Array.from(siblings).indexOf(el);
    const delays = ['reveal-d1','reveal-d2','reveal-d3','reveal-d4','reveal-d5'];
    if (idx < delays.length) el.classList.add(delays[idx]);
  }
  revealObserver.observe(el);
});

/* ── Topic accordion ────────────────────────────────────────────────────── */
function toggleTopic(id) {
  const card = document.getElementById(id);
  card.classList.toggle('open');
}

/* ── App Preview screen switcher ────────────────────────────────────────── */
function switchScreen(name, btn) {
  document.querySelectorAll('.screen-panel').forEach(p => p.classList.remove('active'));
  document.querySelectorAll('.screen-tab').forEach(t => t.classList.remove('active'));
  document.getElementById('panel-' + name).classList.add('active');
  if (btn) btn.classList.add('active');
}

/* ── Chat demo ──────────────────────────────────────────────────────────── */
const demoReplies = {
  'counseling': "BMCC's Counseling Center offers free, confidential sessions at Room S-343.\n📞 (212) 220-8140 · Mon, Tue, Thu 9am–6pm · Wed, Fri 9am–5pm\nYou can also book via Zoom. No referral needed.",
  'tutor': "The Learning Resource Center has free peer tutoring — no appointment needed!\n📍 Room S-510, 199 Chambers St.\n🕐 Mon–Thu 10am–6pm · Fri–Sat 10am–5pm\nYou can also book online at bmcc.upswing.io",
  'food': "The Panther Pantry has free food for all BMCC students — just walk in.\n📍 Room S-230 · Mon–Fri 8am–6:30pm · Sat 10am–1pm\nNo paperwork or questions asked.",
  'sleep': "A few things that help: aim for 7–9 hours, keep consistent bed/wake times, avoid screens 30 min before sleep, and try the 4-7-8 breathing technique (inhale 4s, hold 7s, exhale 8s). The Sleep folder in the Toolkit has a sleep calculator too!",
  'stress': "Stress during the semester is normal, but you don't have to carry it alone.\nTry: breaking tasks into small steps · using the LRC for academic support · speaking with a BMCC counselor · trying a short mindfulness practice.\nWhat's stressing you most right now?",
  'anxiety': "Anxiety can feel overwhelming, but small grounding steps really help.\nTry the 5-4-3-2-1 method: name 5 things you see, 4 you hear, 3 you can touch, 2 you smell, 1 you taste.\nIf anxiety is persistent, BMCC's free counseling can help: (212) 220-8140.",
  'immigration': "BMCC has a dedicated Immigrant Resource Center — confidential for all statuses.\n📞 Albert Lee: (212) 776-6252 · allee@bmcc.cuny.edu\n📍 Room S-230\nFree legal services (DACA, TPS, naturalization) are also available monthly.",
  'default': "That's a thoughtful question. MindEase can help you explore BMCC resources, wellness tools, and coping strategies. Try asking about tutoring, counseling, food, sleep, stress, anxiety, or immigration support — I'm here for all of it."
};

function getBotReply(text) {
  const t = text.toLowerCase();
  if (t.includes('counsel') || t.includes('therapy') || t.includes('mental health')) return demoReplies.counseling;
  if (t.includes('tutor') || t.includes('study') || t.includes('exam') || t.includes('class')) return demoReplies.tutor;
  if (t.includes('food') || t.includes('eat') || t.includes('hungry') || t.includes('pantry')) return demoReplies.food;
  if (t.includes('sleep') || t.includes('tired') || t.includes('rest')) return demoReplies.sleep;
  if (t.includes('stress') || t.includes('overwhelm') || t.includes('pressure')) return demoReplies.stress;
  if (t.includes('anxiet') || t.includes('panic') || t.includes('worry') || t.includes('nervous')) return demoReplies.anxiety;
  if (t.includes('immig') || t.includes('daca') || t.includes('undocument') || t.includes('visa')) return demoReplies.immigration;
  return demoReplies.default;
}

function addMsg(text, role) {
  const wrap = document.createElement('div');
  wrap.className = 'msg ' + role;
  wrap.innerHTML = `<div class="msg-icon">${role === 'bot' ? '🌿' : '🙋'}</div><div class="msg-bubble">${text.replace(/\n/g,'<br>')}</div>`;
  const msgs = document.getElementById('chat-messages');
  msgs.appendChild(wrap);
  msgs.scrollTop = msgs.scrollHeight;
  return wrap;
}

function showTyping() {
  const wrap = document.createElement('div');
  wrap.className = 'msg bot typing-wrap';
  wrap.id = 'typing-indicator';
  wrap.innerHTML = `<div class="msg-icon">🌿</div><div class="msg-bubble"><span class="typing-dot"></span><span class="typing-dot"></span><span class="typing-dot"></span></div>`;
  const msgs = document.getElementById('chat-messages');
  msgs.appendChild(wrap);
  msgs.scrollTop = msgs.scrollHeight;
}

function hideTyping() {
  const t = document.getElementById('typing-indicator');
  if (t) t.remove();
}

function sendMessage() {
  const input = document.getElementById('chat-input');
  const text = input.value.trim();
  if (!text) return;
  addMsg(text, 'user');
  input.value = '';
  showTyping();
  setTimeout(() => { hideTyping(); addMsg(getBotReply(text), 'bot'); }, 1100);
}

document.getElementById('chat-input').addEventListener('keydown', e => {
  if (e.key === 'Enter') sendMessage();
});

/* ── Journal demo ───────────────────────────────────────────────────────── */
const today = new Date();
document.getElementById('j-date').textContent = 'Today, ' + today.toLocaleDateString('en-US', { month:'long', day:'numeric', year:'numeric' });

document.addEventListener('keydown', e => {
  if ((e.ctrlKey || e.metaKey) && e.key === 's') { e.preventDefault(); saveJournalEntry(); }
});

function updateWordCount() {
  const body = document.getElementById('j-body').value.trim();
  const words = body ? body.split(/\s+/).filter(Boolean).length : 0;
  document.getElementById('j-wc').textContent = words + (words === 1 ? ' word' : ' words');
}

function saveJournalEntry() {
  const title = document.getElementById('j-title').value.trim();
  const body  = document.getElementById('j-body').value.trim();
  if (!body) { showJStatus('Write something before saving.', false); return; }

  const card = document.createElement('div');
  card.className = 'entry-card';
  const displayTitle = title || body.split('\n')[0].slice(0, 56) || 'Untitled reflection';
  const excerpt = body.length > 180 ? body.slice(0, 180) + '…' : body;
  const now = new Date();
  const dateStr = now.toLocaleDateString('en-US', { month:'short', day:'numeric', year:'numeric' }) + ' · ' +
                  now.toLocaleTimeString('en-US', { hour:'numeric', minute:'2-digit' });
  card.setAttribute('data-text', body.toLowerCase());
  card.innerHTML = `<div class="entry-title">${escHtml(displayTitle)}</div>
    <div class="entry-excerpt">${escHtml(excerpt)}</div>
    <div class="entry-date">${dateStr}</div>`;

  const panel = document.getElementById('entries-panel');
  panel.insertBefore(card, panel.firstChild);
  // animate in
  card.style.opacity = '0'; card.style.transform = 'translateY(-10px)';
  card.style.transition = 'opacity 0.35s, transform 0.35s';
  requestAnimationFrame(() => { card.style.opacity='1'; card.style.transform='none'; });

  document.getElementById('j-title').value = '';
  document.getElementById('j-body').value = '';
  updateWordCount();
  showJStatus('✓ Entry saved successfully!');
}

function showJStatus(msg, ok = true) {
  const el = document.getElementById('j-status');
  el.textContent = msg;
  el.style.color = ok ? '#2E7D32' : '#C62828';
  setTimeout(() => { if (el.textContent === msg) el.textContent = ''; }, 4000);
}

function escHtml(s) {
  return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
}

function filterEntries() {
  const q = document.getElementById('j-search').value.trim().toLowerCase();
  document.querySelectorAll('#entries-panel .entry-card').forEach(card => {
    const text = (card.getAttribute('data-text') || '') + card.innerText.toLowerCase();
    card.style.display = (!q || text.includes(q)) ? '' : 'none';
  });
}
