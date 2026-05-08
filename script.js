/* ── Falling Leaves ─────────────────────────────────────────────────────── */
(function() {
  const canvas = document.getElementById('leaf-canvas');
  const leafSvgs = [
    '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"><path d="M11 20A7 7 0 0 1 9.8 6.1C15.5 5 17 4.48 19 2c1 2 2 4.18 2 8 0 5.5-4.78 10-10 10Z"/><path d="M2 21c0-3 1.85-5.36 5.08-6C9.5 14.52 12 13 13 12"/></svg>',
    '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"><path d="M11 20A7 7 0 0 1 9.8 6.1C15.5 5 17 4.48 19 2c1 2 2 4.18 2 8 0 5.5-4.78 10-10 10Z"/></svg>',
    '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"><path d="M12 2a10 10 0 0 1 10 10c0 5.52-4.48 10-10 10S2 17.52 2 12"/><path d="M2 12C2 6.48 6.48 2 12 2"/></svg>',
  ];
  const count = 18;
  for (let i = 0; i < count; i++) {
    const leaf = document.createElement('div');
    leaf.className = 'leaf';
    const size = 16 + Math.random() * 12;
    leaf.innerHTML = leafSvgs[Math.floor(Math.random() * leafSvgs.length)];
    leaf.style.cssText = `
      left: ${Math.random() * 100}%;
      width: ${size}px;
      height: ${size}px;
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
  darkToggle.innerHTML = d
    ? '<i data-lucide="sun"></i> Light'
    : '<i data-lucide="moon"></i> Night';
  if (typeof lucide !== 'undefined') lucide.createIcons({ nodes: [darkToggle] });
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
  '.eyebrow, .section-title, .section-sub, .feature-card, .topic-card, .project-card, .intro-card, .reveal'
).forEach((el, i) => {
  if (!el.classList.contains('reveal')) el.classList.add('reveal');
  // stagger children inside grids
  const parent = el.closest('.features-grid, .topic-grid, .project-cards');
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
  const empty = panel.querySelector('.empty-entries');
  if (empty) empty.remove();
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

lucide.createIcons();
