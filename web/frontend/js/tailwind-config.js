// Palette et typographie du dashboard. Chargé juste après le CDN Tailwind
// (avant tout le reste) pour que les classes personnalisées utilisées dans
// index.html (bg-emerald-950, font-mono, shadow-crisp...) soient reconnues.
tailwind.config = {
  theme: {
    extend: {
      colors: {
        paper: '#FBFDFB',
        paperAlt: '#F1F8F4',
        ink: '#0B1F16',
        inkSoft: '#4B6C5D',
        border: '#DCEAE2',
        emerald: {
          950: '#052E1D',
          900: '#0B3B26',
          700: '#0F7A4F',
          600: '#129A62',
          500: '#16B876',
        },
        neon: '#22FF88',
      },
      fontFamily: {
        grotesk: ['"Space Grotesk"', 'sans-serif'],
        mono: ['"JetBrains Mono"', 'monospace'],
      },
      boxShadow: {
        crisp: '0 1px 1px rgba(6,78,47,0.05), 0 10px 20px -14px rgba(6,78,47,0.28)',
      },
    },
  },
};
