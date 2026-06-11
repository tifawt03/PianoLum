#pragma once
#include <pgmspace.h>
const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0,viewport-fit=cover">
<meta http-equiv="Cache-Control" content="no-store">
<title>PianoLUM</title>
<link rel="icon" href="data:,">
<style>
*{margin:0;padding:0;box-sizing:border-box;}
html,body{height:100%;}
body{
  min-height:100%;display:flex;align-items:center;justify-content:center;
  background:radial-gradient(120% 80% at 50% 0%,#20203a,#0d0d18 70%);
  font-family:-apple-system,system-ui,sans-serif;
  padding:32px 16px 56px;
}
.phone{
  position:relative;width:402px;height:874px;
  border-radius:56px;background:#0a0a0f;padding:11px;
  box-shadow:0 0 0 2px #2a2a33,0 0 0 6px #0a0a0f,
             0 40px 90px rgba(0,0,0,.55),inset 0 0 1px rgba(255,255,255,.2);
}
.screen{
  position:relative;width:100%;height:100%;border-radius:46px;
  overflow:hidden;background:#1A1A2E;isolation:isolate;
}
.island{
  position:absolute;top:11px;left:50%;transform:translateX(-50%);
  width:122px;height:35px;border-radius:20px;background:#000;z-index:40;
}
.statusbar{
  position:absolute;top:0;left:0;right:0;height:54px;z-index:35;
  display:flex;align-items:center;justify-content:space-between;
  padding:18px 32px 0;pointer-events:none;color:#fff;
}
.statusbar .time{font-size:16px;font-weight:600;letter-spacing:.2px;}
.statusbar .icons{display:flex;align-items:center;gap:7px;}
.home-ind{
  position:absolute;bottom:8px;left:50%;transform:translateX(-50%);
  width:138px;height:5px;border-radius:100px;
  background:rgba(255,255,255,.85);z-index:45;pointer-events:none;
}
.caption{
  position:fixed;left:0;right:0;bottom:18px;text-align:center;
  color:rgba(232,224,208,.45);font-size:12px;letter-spacing:.18em;
  text-transform:uppercase;font-weight:600;
}

:root{
  --bg:#1A1A2E; --bg2:#15152a; --header:#111;
  --ivory:#e8e0d0; --cyan:#76CCD6;
  --led-blue:#3b8df5; --led-orange:#ff8c00; --gold:#F2C24B;
  --wk:#efe9dd; --wk2:#d9d2c2; --bk:#15151f; --bk2:#2a2a3a;
  --font:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,system-ui,sans-serif;
}
*{-webkit-tap-highlight-color:transparent;}
body{font-family:var(--font);background:var(--bg);color:var(--ivory);
     overflow:hidden;-webkit-user-select:none;user-select:none;
     -webkit-font-smoothing:antialiased;touch-action:manipulation;}

.app{
  position:absolute;inset:0;display:flex;flex-direction:column;
  background:radial-gradient(120% 60% at 50% -10%,rgba(118,204,214,.12),transparent 60%),
             linear-gradient(180deg,var(--bg) 0%,var(--bg2) 100%);
}

/* ---- HEADER ---- */
header{
  background:var(--header);padding:46px 18px 12px;
  display:flex;align-items:center;gap:12px;
  border-bottom:1px solid rgba(255,255,255,.06);
  box-shadow:0 6px 22px rgba(0,0,0,.5);z-index:5;
}
.logo{height:44px;width:auto;flex:none;
      filter:drop-shadow(0 0 9px rgba(118,204,214,.55));}
.brand{display:flex;flex-direction:column;line-height:1;}
.brand .word{font-size:20px;letter-spacing:.42em;display:flex;}
.brand .word .l{font-weight:300;color:var(--ivory);}
.brand .word .b{font-weight:800;color:#fff;}
.brand .sub{margin-top:7px;font-size:9px;font-weight:600;letter-spacing:.28em;
            text-transform:uppercase;color:var(--cyan);opacity:.85;}

/* ---- MAIN ---- */
main{flex:1;min-height:0;display:flex;flex-direction:column;padding:12px 14px 4px;gap:10px;}

/* toggle */
.toggle-row{
  flex:none;display:flex;align-items:center;justify-content:space-between;gap:14px;
  background:rgba(255,255,255,.04);border:1px solid rgba(255,255,255,.08);
  border-radius:14px;padding:11px 14px 11px 16px;
}
.toggle-row .tl{display:flex;flex-direction:column;gap:2px;min-width:0;}
.toggle-row .t-title{font-size:13px;font-weight:700;color:var(--ivory);
                     display:flex;align-items:center;gap:7px;}
.toggle-row .t-title svg{width:14px;height:14px;color:var(--cyan);flex:none;}
.toggle-row .t-sub{font-size:10.5px;font-weight:300;color:rgba(232,224,208,.55);}
.switch{
  position:relative;width:50px;height:29px;flex:none;border:none;cursor:pointer;
  border-radius:999px;background:rgba(255,255,255,.16);transition:.25s ease;
}
.switch::after{
  content:"";position:absolute;top:3px;left:3px;width:23px;height:23px;
  border-radius:50%;background:#fff;box-shadow:0 2px 5px rgba(0,0,0,.35);
  transition:.25s ease;
}
.switch.on{background:var(--cyan);box-shadow:0 0 14px rgba(118,204,214,.5);}
.switch.on::after{transform:translateX(21px);}

/* filtres */
.filters{display:flex;gap:7px;overflow-x:auto;scrollbar-width:none;padding-bottom:2px;}
.filters::-webkit-scrollbar{display:none;}
.pill{
  flex:none;font-family:var(--font);font-size:11.5px;font-weight:700;
  letter-spacing:.03em;color:var(--ivory);background:rgba(255,255,255,.05);
  border:1px solid rgba(255,255,255,.10);padding:8px 14px;border-radius:999px;
  cursor:pointer;transition:.2s ease;white-space:nowrap;
}
.pill:active{transform:scale(.95);}
.pill.active{
  background:linear-gradient(180deg,var(--cyan),#5bb6c1);
  color:#0f2024;border-color:transparent;
  box-shadow:0 4px 14px rgba(118,204,214,.35);
}

/* carte sélection */
.selection{
  flex:none;background:rgba(255,255,255,.04);border:1px solid rgba(255,255,255,.09);
  border-radius:16px;padding:14px 16px;min-height:100px;
  display:flex;flex-direction:column;justify-content:center;
  position:relative;overflow:hidden;
}
.selection::before{
  content:"";position:absolute;left:0;top:0;bottom:0;width:4px;
  background:var(--cyan);opacity:0;transition:.25s;
}
.selection.has::before{opacity:1;box-shadow:0 0 12px var(--cyan);}
.sel-empty{display:flex;flex-direction:column;gap:5px;}
.sel-empty .big{font-size:14px;font-weight:700;color:var(--ivory);}
.sel-empty .small{font-size:12px;font-weight:300;color:rgba(232,224,208,.65);line-height:1.5;}
.sel-card{display:none;}
.selection.has .sel-card{display:block;}
.selection.has .sel-empty{display:none;}
.sel-top{display:flex;align-items:flex-start;justify-content:space-between;gap:10px;}
.sel-title{font-size:21px;font-weight:800;color:#fff;line-height:1.1;}
.sel-keytag{
  flex:none;font-size:10.5px;font-weight:800;letter-spacing:.06em;white-space:nowrap;
  padding:5px 9px;border-radius:7px;background:rgba(255,255,255,.07);
  border:1px solid rgba(255,255,255,.13);color:var(--cyan);
}
.sel-keytag.orange{color:var(--led-orange);}
.sel-meta{display:flex;align-items:center;gap:12px;margin-top:10px;}
.stars{display:flex;gap:3px;}
.star{width:16px;height:16px;}
.star.on{fill:var(--gold);filter:drop-shadow(0 0 4px rgba(242,194,75,.6));}
.star.off{fill:rgba(232,224,208,.16);}
.sel-info{
  display:flex;align-items:center;gap:7px;
  font-size:12px;font-weight:600;color:rgba(232,224,208,.82);white-space:nowrap;
}
.sel-info .dot-sep{width:3px;height:3px;border-radius:50%;background:rgba(232,224,208,.4);}
.sel-info .lvl{color:var(--cyan);}

/* clavier */
.keyboard-zone{flex:1;min-height:0;display:flex;flex-direction:column;}
.kb-hint{
  flex:none;text-align:center;font-size:9.5px;font-weight:600;letter-spacing:.2em;
  text-transform:uppercase;color:rgba(232,224,208,.45);margin-bottom:7px;
}
.keyboard{
  flex:1;min-height:160px;width:100%;
  overflow-y:auto;overflow-x:hidden;-webkit-overflow-scrolling:touch;
  border-radius:10px;background:#0c0c16;padding:6px;
  box-shadow:inset 0 0 0 1px rgba(118,204,214,.14),0 12px 28px rgba(0,0,0,.5);
  scrollbar-width:none;
}
.keyboard::-webkit-scrollbar{display:none;}
.kb-inner{position:relative;}
.white-col{display:flex;flex-direction:column;gap:3px;}
.black-layer{position:absolute;inset:0;z-index:2;pointer-events:none;}

.key{
  position:relative;border:none;cursor:pointer;font-family:var(--font);
  padding:0;transition:transform .07s ease,opacity .22s ease;
  --led:var(--led-blue);
}
.key.col-orange{--led:var(--led-orange);}
.key.white{
  height:62px;width:100%;
  display:flex;align-items:center;justify-content:flex-end;gap:10px;
  padding-right:18px;border-radius:0 6px 6px 0;
  background:linear-gradient(90deg,var(--wk2) 0%,var(--wk) 18%,var(--wk) 100%);
  box-shadow:inset 0 -2px 3px rgba(0,0,0,.14),inset 7px 0 9px rgba(0,0,0,.07);
}
.key.black{
  position:absolute;left:0;width:58%;height:42px;
  pointer-events:auto;z-index:3;
  display:flex;align-items:center;justify-content:flex-end;gap:8px;
  padding-right:14px;border-radius:0 5px 5px 0;
  background:linear-gradient(90deg,#0a0a12 0%,var(--bk) 68%,var(--bk2) 100%);
  box-shadow:4px 0 6px rgba(0,0,0,.55),inset -4px 0 4px rgba(0,0,0,.6),
             inset 0 1px 0 rgba(255,255,255,.06);
}
.key.white:active{transform:translateX(-3px);}
.key.black:active{transform:translateX(-3px);}

.klabel{
  font-size:13px;font-weight:700;white-space:nowrap;color:#2a2535;
  pointer-events:none;position:relative;z-index:1;
}
.key.black .klabel{color:var(--ivory);font-size:12px;font-weight:600;}
.dot{
  flex:none;width:11px;height:11px;border-radius:50%;
  background:var(--led);box-shadow:0 0 8px var(--led);
  position:relative;z-index:1;transition:.25s;
}
.key.black .dot{width:8px;height:8px;}
.key.dimmed{opacity:.32;pointer-events:none;}
.key.dimmed .dot{filter:grayscale(1);box-shadow:none;}

/* barre LED gauche */
.led{
  position:absolute;left:0;top:0;bottom:0;width:7px;
  display:none;flex-direction:column;gap:2px;padding:4px 0;
  border-radius:5px 0 0 5px;background:rgba(0,0,0,.3);
  overflow:hidden;z-index:2;
}
.key.black .led{width:5px;}
.key.selected .led{display:flex;animation:ledGlow 1.5s ease-in-out infinite;}
.led .seg{
  flex:1;margin:1px 0;border-radius:2px;background:var(--led);
  animation:segChase 1.15s ease-in-out infinite;
}
.key.white.selected{
  background:linear-gradient(90deg,var(--wk2) 0%,#fff 18%,#fff 100%);
  box-shadow:inset 0 -2px 3px rgba(0,0,0,.12),
             0 0 0 1px var(--led),0 0 20px -2px var(--led);
  transform:translateX(-3px);
}
.key.black.selected{
  box-shadow:4px 0 6px rgba(0,0,0,.55),0 0 0 1px var(--led),0 0 18px -2px var(--led);
  transform:translateX(-3px);
}
.key.selected .dot{box-shadow:0 0 12px var(--led),0 0 4px #fff;}

@keyframes segChase{0%,100%{opacity:.22;transform:scaleY(.55);}50%{opacity:1;transform:scaleY(1);}}
@keyframes ledGlow{0%,100%{box-shadow:0 0 6px var(--led),0 0 12px -2px var(--led);}50%{box-shadow:0 0 12px var(--led),0 0 26px -2px var(--led);}}

/* ---- FOOTER ---- */
footer{
  flex:none;padding:8px 14px calc(env(safe-area-inset-bottom) + 12px);
  background:linear-gradient(180deg,transparent,rgba(0,0,0,.25));
}
.launch{
  width:100%;border:none;cursor:pointer;font-family:var(--font);
  font-size:15px;font-weight:800;letter-spacing:.04em;color:#0c1a1d;
  padding:16px;border-radius:14px;
  display:flex;align-items:center;justify-content:center;gap:9px;
  background:linear-gradient(180deg,#86d8e2,#5bb6c1);
  box-shadow:0 7px 20px rgba(118,204,214,.4);transition:.18s ease;
}
.launch:active{transform:translateY(2px);}
.launch:disabled{background:rgba(255,255,255,.06);color:rgba(232,224,208,.32);
                 box-shadow:none;cursor:not-allowed;}
.launch svg{width:17px;height:17px;flex:none;}

/* ---- NOW PLAYING ---- */
.nowplaying{display:none;flex-direction:column;gap:10px;}
.app.playing .nowplaying{display:flex;}
.app.playing .launch{display:none;}
.app.playing .keyboard{opacity:.5;pointer-events:none;filter:saturate(.7);}

.np-card{
  background:linear-gradient(180deg,rgba(118,204,214,.10),rgba(118,204,214,.04));
  border:1px solid rgba(118,204,214,.38);border-radius:14px;padding:13px 15px;
  display:flex;flex-direction:column;gap:10px;
  box-shadow:0 0 20px -6px rgba(118,204,214,.4);
}
.np-head{display:flex;align-items:center;justify-content:space-between;gap:10px;}
.np-now{display:flex;align-items:center;gap:9px;min-width:0;}
.np-eq{display:flex;align-items:flex-end;gap:2px;height:18px;width:18px;flex:none;}
.np-eq span{flex:1;background:var(--cyan);border-radius:1px;height:30%;
            animation:eq .9s ease-in-out infinite;}
.np-eq span:nth-child(2){animation-delay:.18s;}
.np-eq span:nth-child(3){animation-delay:.36s;}
.np-eq span:nth-child(4){animation-delay:.54s;}
@keyframes eq{0%,100%{height:25%;}50%{height:100%;}}
.np-title{font-size:15px;font-weight:800;color:#fff;
          white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}
.np-count{font-size:11px;font-weight:700;color:var(--cyan);white-space:nowrap;}
.np-count b{font-size:15px;font-weight:800;}
.app.done .np-eq{display:none;}
.app.done .np-count{color:var(--gold);}
.np-bar{display:flex;gap:2px;height:8px;align-items:stretch;}
.np-bar .seg{flex:1;min-width:0;border-radius:2px;background:rgba(255,255,255,.12);
             transition:background .15s,box-shadow .15s;}
.np-bar .seg.done{background:var(--cyan);}
.np-bar .seg.cur{background:#fff;box-shadow:0 0 8px var(--cyan);}

.np-actions{display:flex;gap:9px;}
.np-btn{
  flex:1;border:none;cursor:pointer;font-family:var(--font);
  font-size:13.5px;font-weight:800;padding:13px;border-radius:11px;
  display:flex;align-items:center;justify-content:center;gap:7px;transition:.16s ease;
}
.np-btn:active{transform:translateY(2px);}
.np-btn svg{width:15px;height:15px;flex:none;}
.np-restart{background:rgba(255,255,255,.07);color:var(--ivory);
            border:1px solid rgba(255,255,255,.14);}
.np-stop{background:linear-gradient(180deg,#e8576b,#d23b50);color:#fff;
         box-shadow:0 5px 16px rgba(210,59,80,.35);}

/* ---- TOAST ---- */
.toast{
  position:absolute;left:50%;bottom:88px;transform:translate(-50%,16px);
  background:rgba(12,12,22,.96);border:1px solid rgba(118,204,214,.4);
  color:var(--ivory);padding:11px 18px;border-radius:11px;
  font-size:13px;font-weight:700;display:flex;align-items:center;gap:8px;
  box-shadow:0 10px 28px rgba(0,0,0,.6);
  opacity:0;pointer-events:none;transition:.3s ease;z-index:50;white-space:nowrap;
}
.toast.show{opacity:1;transform:translate(-50%,0);}
.toast .note{color:var(--cyan);font-size:15px;}

@media (max-height:680px){main{gap:7px;}
  .selection{min-height:88px;padding:11px 14px;}.sel-title{font-size:18px;}}

@media(max-width:450px){
  body{padding:0;display:block;background:var(--bg);}
  .phone{width:100%;height:100%;height:100dvh;border-radius:0;padding:0;
         box-shadow:none;background:transparent;}
  .screen{border-radius:0;background:var(--bg);}
  .island,.statusbar,.home-ind,.caption{display:none!important;}
  header{padding:calc(env(safe-area-inset-top,20px) + 12px) 18px 12px;}
}
</style>
</head>
<body>
<div class="phone">
  <div class="screen">
    <div class="island"></div>
    <div class="statusbar">
      <span class="time">9:41</span>
      <span class="icons">
        <svg width="17" height="12" viewBox="0 0 18 12"><rect x="0" y="7.5" width="3" height="4.5" rx=".6" fill="#fff"/><rect x="4.5" y="5" width="3" height="7" rx=".6" fill="#fff"/><rect x="9" y="2.5" width="3" height="9.5" rx=".6" fill="#fff"/><rect x="13.5" y="0" width="3" height="12" rx=".6" fill="#fff"/></svg>
        <svg width="16" height="12" viewBox="0 0 17 12"><path d="M8.5 3.2C10.8 3.2 12.9 4.1 14.4 5.6L15.5 4.5C13.7 2.7 11.2 1.5 8.5 1.5 5.8 1.5 3.3 2.7 1.5 4.5L2.6 5.6C4.1 4.1 6.2 3.2 8.5 3.2Z" fill="#fff"/><path d="M8.5 6.8C9.9 6.8 11.1 7.3 12 8.2L13.1 7.1C11.8 5.9 10.2 5.1 8.5 5.1 6.8 5.1 5.2 5.9 3.9 7.1L5 8.2C5.9 7.3 7.1 6.8 8.5 6.8Z" fill="#fff"/><circle cx="8.5" cy="10.5" r="1.4" fill="#fff"/></svg>
        <svg width="25" height="12" viewBox="0 0 27 13"><rect x=".5" y=".5" width="23" height="12" rx="3.5" stroke="#fff" stroke-opacity=".4" fill="none"/><rect x="2" y="2" width="19" height="9" rx="2" fill="#fff"/><path d="M25 4.5V8.5C25.8 8.2 26.5 7.2 26.5 6.5 26.5 5.8 25.8 4.8 25 4.5Z" fill="#fff" fill-opacity=".5"/></svg>
      </span>
    </div>

    <div class="app" id="app">

    <!-- HEADER — logo SVG inline (pas de fichier externe) -->
    <header>
      <svg class="logo" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 682.26 802.82">
        <path fill="#e8e0d0" d="M478.17,480.31c-3.48,9.16-11.86,18.02-22.3,18.03l-184.53.19c-20.48.02-27.04-17.59-28.7-34.85-4.26-44.29-19.98-84.47-44.66-121.23-16.11-24-29.22-49.24-36.47-77.49-11.07-43.12-8.3-88.86,8.11-130.39,16.32-41.31,45.35-76.72,82.16-101.27C293.35,5.57,343.5-5.69,392.71,2.74c52.68,9.03,98.89,38.37,131.62,80.12,30.4,38.77,46.51,85.91,45.02,135.39-1.46,48.38-15.39,82.16-41.43,122-8.18,12.52-16,24.16-22.59,37.6-23.44,47.78-20.59,85.21-27.15,102.47ZM271.44,462.06l-.02-160.82-7.73-.91c-4.94-.58-8.63-3.57-8.64-9.19l-.08-130.76-24.15-.06c-9.65-.02-19.73,7.3-19.74,18.07l-.09,148.2,10.97,18.71c20.44,34.87,35.12,72.28,40.1,112.4.56,4.54,6.33,7.7,9.37,4.35ZM353.01,463.19v-161.94s-7.33-.86-7.33-.86c-4.72-.55-8.67-3.73-8.67-9.23l-.02-130.79-31.71.07v131.32c0,4.97-3.68,8.03-8.02,8.57l-8.26,1.02.03,161.93,63.99-.1ZM435.66,463.2l-.09-161.92-8.06-.9c-4.36-.48-8.5-2.97-8.5-8.24l-.03-131.75-33.62-.03-.06,132.25c0,9.39-13.87,6.78-15.06,9.56l.05,161.1,65.35-.08ZM483.87,378.95c4.72-10.84,10.26-20.64,16.22-30.68l12.9-21.73-.1-149.15c0-9.6-9.35-16.95-17.94-16.96l-25.27-.03-.03,131.72c0,4.46-3.64,7.63-7.47,8.09l-9.01,1.08.03,161.62c4.46,1.49,8.23-1.15,8.95-5.74,4.25-27.12,10.75-53.05,21.71-78.23Z"/>
        <g fill="#e8e0d0">
          <polygon points="662.08 801.3 661.86 737.87 638.98 785.28 628.42 785.47 605.08 737.71 604.94 801.21 585 801.16 585.01 707.35 611.34 707.4 633.64 753.43 655.79 707.49 682.24 707.57 682.26 800.93 662.08 801.3"/>
          <polygon points="276.01 801.07 255.55 801.32 222.13 740.93 221.93 801.19 202.45 801.25 202.41 707.35 224.99 707.31 256.02 763.89 256.42 707.39 275.98 707.32 276.01 801.07"/>
          <path d="M374.76,770.25c-6.16,26.73-31.04,37.03-55.81,30.79-15.13-3.81-26.15-16.08-29.12-31.38-2.12-10.95-1.74-22.22.67-32.84,4.58-20.15,21.49-31.42,41.48-31.46,20.08-.04,36.94,10.69,42.24,30.71,2.89,10.92,3.14,22.87.53,34.18ZM333.42,784.22c9.91-.22,17.17-5.94,19.82-15.06s2.58-19.46.02-28.68c-2.83-10.2-10.88-16.22-20.74-16.26-9.92-.05-17.85,6.09-20.69,16.21-2.69,9.58-2.51,19.74.47,29.36s11,14.66,21.13,14.44Z"/>
          <path d="M548.46,707.38l20.87.06-.18,64.94c-.04,13.89-9.16,25.23-22.28,28.51-10.26,2.56-20.46,2.62-30.7-.01-12.32-3.17-21.77-13.45-21.83-26.76l-.31-66.77,20.93-.04.15,62.47c.02,9.09,7.22,14.78,15.93,14.63,4.65-.08,9.41-.94,12.63-3.98,2.86-2.7,4.66-7.59,4.67-12.13l.14-60.91Z"/>
          <path d="M165.7,781.02l-30.86.02-6.7,20.15h-21.71s31.57-93.85,31.57-93.85l24.12-.02,31.89,93.81-21.22.18-7.08-20.29ZM160.66,763.35l-10.55-31.56-10.57,31.86,21.12-.3Z"/>
          <path d="M64.35,744.89c-2.9,22.54-23.32,26.96-43.1,25.55l-.12,30.71-21.14.08v-93.94s37.97.17,37.97.17c6.79.03,13.36,3.47,18.35,7.31,7.28,8.6,9.44,19.14,8.03,30.11ZM44.09,737.75c-.04-6.71-3.72-12.29-10.35-12.35l-12.56-.12.02,26.9,11.6-.09c7.52-.06,11.33-6.81,11.29-14.34Z"/>
          <polygon points="484.56 782.15 484.46 801.3 425.59 801.25 425.66 707.46 446.99 707.36 447.01 781.97 484.56 782.15"/>
          <polygon points="97.05 801.17 76.04 801.28 75.99 707.32 96.98 707.36 97.05 801.17"/>
        </g>
        <g fill="#76CCD6">
          <path d="M415.45,599.46c-10.79,20.14-31.03,32.87-52.96,32.86-22.74,0-43.28-12.19-53.8-32.83l-10.82-1.38c-15.08-1.92-26.92-14.54-26.87-29.78.02-6.56,4.81-10.7,11.1-10.7l159.78-.02c6.87,0,11.83,4.91,11.59,11.64-.49,13.81-10.19,25.51-23.8,28.26-4.58.92-8.86,1.34-14.22,1.94Z"/>
          <path d="M448.31,540.77h-173.16c-7.73-.01-12.63-4.6-12.18-12.84.19-3.56,2.53-10.89,7.89-10.89l181.51-.06c5.48,0,8.15,5.75,8.52,10.1.67,7.89-4.01,13.69-12.58,13.69Z"/>
        </g>
      </svg>
      <div class="brand">
        <div class="word"><span class="b">PIANO</span><span class="l">&nbsp;LUM</span></div>
        <div class="sub">Apprentissage guid&#233; par LED</div>
      </div>
    </header>

    <!-- MAIN -->
    <main>
      <div class="toggle-row">
        <div class="tl">
          <div class="t-title">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2 12s3.5-7 10-7 10 7 10 7-3.5 7-10 7-10-7-10-7Z"/><circle cx="12" cy="12" r="3"/></svg>
            Preview prochaine note
          </div>
          <div class="t-sub">Aper&#231;u LED de la note &#224; venir</div>
        </div>
        <button class="switch on" id="previewSwitch" role="switch"
                aria-checked="true" aria-label="Preview prochaine note"></button>
      </div>

      <div class="filters" id="filters">
        <button class="pill active" data-level="all">Tous</button>
        <button class="pill" data-level="D&#xe9;butant">D&#xe9;butant</button>
        <button class="pill" data-level="Interm&#xe9;diaire">Interm&#xe9;diaire</button>
        <button class="pill" data-level="Avanc&#xe9;">Avanc&#xe9;</button>
      </div>

      <div class="selection" id="selection">
        <div class="sel-empty" id="selEmpty">
          <div class="big">Choisis ton morceau &#127929;</div>
          <div class="small" id="selHint">
            Touche une note du clavier pour s&#xe9;lectionner un morceau,
            puis lance l&#x2019;apprentissage.
          </div>
        </div>
        <div class="sel-card" id="selCard">
          <div class="sel-top">
            <div class="sel-title" id="selTitle">&#8212;</div>
            <div class="sel-keytag" id="selKey">DO</div>
          </div>
          <div class="sel-meta">
            <div class="stars" id="selStars"></div>
            <div class="sel-info">
              <span id="selNotes">0 notes</span>
              <span class="dot-sep"></span>
              <span class="lvl" id="selLvl">&#8212;</span>
            </div>
          </div>
        </div>
      </div>

      <div class="keyboard-zone">
        <div class="kb-hint" id="kbHint">
          Chaque touche = un morceau &middot; fais d&#xe9;filer
        </div>
        <div class="keyboard">
          <div class="kb-inner" id="kbInner">
            <div class="white-col" id="whiteCol"></div>
            <div class="black-layer" id="blackLayer"></div>
          </div>
        </div>
      </div>
    </main>

    <!-- FOOTER -->
    <footer>
      <button class="launch" id="launchBtn" disabled>
        <svg viewBox="0 0 24 24" fill="currentColor"><path d="M7 5v14l12-7z"/></svg>
        <span id="launchLabel">S&#xe9;lectionne un morceau</span>
      </button>
      <div class="nowplaying" id="nowplaying">
        <div class="np-card">
          <div class="np-head">
            <div class="np-now">
              <div class="np-eq"><span></span><span></span><span></span><span></span></div>
              <div class="np-title" id="npTitle">&#8212;</div>
            </div>
            <div class="np-count"><b id="npCur">1</b> / <span id="npTotal">0</span></div>
          </div>
          <div class="np-bar" id="npBar"></div>
        </div>
        <div class="np-actions">
          <button class="np-btn np-restart" id="restartBtn">
            <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2"
                 stroke-linecap="round" stroke-linejoin="round">
              <path d="M3 12a9 9 0 1 0 3-6.7"/><path d="M3 4v4h4"/>
            </svg>
            Recommencer
          </button>
          <button class="np-btn np-stop" id="stopBtn">
            <svg viewBox="0 0 24 24" fill="currentColor"><rect x="6" y="6" width="12" height="12" rx="2"/></svg>
            Arr&#234;ter
          </button>
        </div>
      </div>
    </footer>

    <div class="toast" id="toast">
      <span class="note">&#9834;</span><span id="toastMsg"></span>
    </div>

    </div><!-- /.app -->

    <div class="home-ind"></div>
  </div><!-- /.screen -->
</div><!-- /.phone -->
<div class="caption">PianoLUM &middot; 192.168.4.1</div>

<script>
// =====================================================================
// PianoLUM v10 \u2014 JavaScript 100% ASCII (Unicode escapes pour les accents)
// =====================================================================
(function() {

  // --- Donn\u00e9es (level/note/name avec \uXXXX, z\u00e9ro UTF-8 natif) ---
  var SONGS = {
    twinkle: { name:"Twinkle Twinkle",                  notes:14, stars:1,
               level:"D\u00e9butant",      color:"blue",   note:"DO" },
    joyeux:  { name:"Joyeux Anniversaire",              notes:25, stars:2,
               level:"D\u00e9butant",      color:"blue",   note:"R\u00c9" },
    ode:     { name:"Ode \u00e0 la Joie",               notes:30, stars:2,
               level:"D\u00e9butant",      color:"blue",   note:"MI" },
    bella:   { name:"Bella Ciao",                       notes:37, stars:2,
               level:"Interm\u00e9diaire", color:"blue",   note:"FA" },
    jingle:  { name:"Jingle Bells",                     notes:49, stars:3,
               level:"Interm\u00e9diaire", color:"blue",   note:"SOL" },
    harry:   { name:"Harry Potter",                     notes:29, stars:2,
               level:"D\u00e9butant",      color:"orange", note:"DO\u266f" },
    elise:   { name:"Lettre \u00e0 \u00c9lise",         notes:33, stars:2,
               level:"D\u00e9butant",      color:"orange", note:"FA\u266f" }
  };

  var WHITE_ORDER = ["twinkle","joyeux","ode","bella","jingle"];
  var BLACK_ORDER = [
    { song:"harry", after:0 },
    { song:"elise", after:2 }
  ];

  // --- \u00c9tat ---
  var selectedSong = null;
  var previewOn    = true;
  var playingSong  = null;
  var progTimer    = null;

  // --- DOM ---
  var app          = document.getElementById("app");
  var whiteCol     = document.getElementById("whiteCol");
  var blackLayer   = document.getElementById("blackLayer");
  var kbInner      = document.getElementById("kbInner");
  var launchBtn    = document.getElementById("launchBtn");
  var launchLabel  = document.getElementById("launchLabel");
  var selEl        = document.getElementById("selection");
  var selTitle     = document.getElementById("selTitle");
  var selKey       = document.getElementById("selKey");
  var selStars     = document.getElementById("selStars");
  var selNotes     = document.getElementById("selNotes");
  var selLvl       = document.getElementById("selLvl");
  var selHint      = document.getElementById("selHint");
  var npTitle      = document.getElementById("npTitle");
  var npTotal      = document.getElementById("npTotal");
  var npCur        = document.getElementById("npCur");
  var npBar        = document.getElementById("npBar");
  var toastEl      = document.getElementById("toast");
  var toastMsg     = document.getElementById("toastMsg");

  // --- Helpers ---
  function starSVG(on) {
    return '<svg class="star ' + (on ? "on" : "off") + '" viewBox="0 0 24 24">'
         + '<path d="M12 2l2.9 6.2 6.8.9-5 4.7 1.3 6.8L12 17.8 5.7 20.6 7 13.8 2 9.1l6.8-.9z"/>'
         + '</svg>';
  }

  function ledSegments() {
    var s = "";
    for (var i = 0; i < 6; i++) {
      s += '<span class="seg" style="animation-delay:' + (i * 0.13).toFixed(2) + 's"></span>';
    }
    return s;
  }

  function makeKey(songId, isBlack) {
    var song = SONGS[songId];
    var k = document.createElement("button");
    k.className = "key " + (isBlack ? "black" : "white")
                + " " + (song.color === "orange" ? "col-orange" : "col-blue");
    k.setAttribute("data-song", songId);
    k.setAttribute("aria-label", song.name);
    k.innerHTML = '<div class="led">' + ledSegments() + '</div>'
                + '<span class="klabel"></span>'
                + '<span class="dot"></span>';
    // textContent pour le label = s\u00fbr avec accents
    k.querySelector(".klabel").textContent = song.name;
    k.addEventListener("click", function() { onKey(k); });
    return k;
  }

  // --- Construction du clavier ---
  var KEYH = 62, GAP = 3, BLKH = 42, STEP = KEYH + GAP;

  for (var wi = 0; wi < WHITE_ORDER.length; wi++) {
    whiteCol.appendChild(makeKey(WHITE_ORDER[wi], false));
  }
  for (var bi = 0; bi < BLACK_ORDER.length; bi++) {
    var bk = makeKey(BLACK_ORDER[bi].song, true);
    bk.style.top = (BLACK_ORDER[bi].after * STEP + KEYH + GAP / 2 - BLKH / 2) + "px";
    blackLayer.appendChild(bk);
  }
  // Hauteur explicite pour le positionnement absolu des touches noires
  kbInner.style.minHeight = (WHITE_ORDER.length * STEP - GAP + 8) + "px";

  // --- Interactions clavier ---
  function onKey(k) {
    if (app.classList.contains("playing")) return;
    if (k.classList.contains("dimmed")) return;
    var id = k.getAttribute("data-song");
    if (selectedSong === id) { deselect(); return; }
    selectedSong = id;
    var keys = document.querySelectorAll(".key.selected");
    for (var i = 0; i < keys.length; i++) keys[i].classList.remove("selected");
    k.classList.add("selected");
    renderSelection();
  }

  function deselect() {
    selectedSong = null;
    var keys = document.querySelectorAll(".key.selected");
    for (var i = 0; i < keys.length; i++) keys[i].classList.remove("selected");
    renderSelection();
  }

  function renderSelection() {
    if (!selectedSong) {
      selEl.classList.remove("has");
      launchBtn.disabled = true;
      launchLabel.textContent = "S\u00e9lectionne un morceau";
      return;
    }
    var song = SONGS[selectedSong];
    selEl.classList.add("has");
    selTitle.textContent = song.name;
    selKey.textContent   = song.note;
    selKey.classList.toggle("orange", song.color === "orange");
    selStars.innerHTML   = starSVG(song.stars >= 1)
                         + starSVG(song.stars >= 2)
                         + starSVG(song.stars >= 3);
    selNotes.textContent = song.notes + " notes";
    selLvl.textContent   = song.level;
    launchBtn.disabled   = false;
    launchLabel.textContent = "Lancer l'apprentissage";
  }

  // --- Filtres ---
  function applyFilter(level) {
    var pills = document.querySelectorAll("#filters .pill");
    for (var i = 0; i < pills.length; i++) {
      pills[i].classList.toggle("active", pills[i].getAttribute("data-level") === level);
    }
    var available = 0;
    var allKeys = document.querySelectorAll(".key[data-song]");
    for (var j = 0; j < allKeys.length; j++) {
      var song  = SONGS[allKeys[j].getAttribute("data-song")];
      var match = (level === "all") || (song.level === level);
      allKeys[j].classList.toggle("dimmed", !match);
      if (match) available++;
    }
    if (selectedSong) {
      var cur = document.querySelector('.key[data-song="' + selectedSong + '"]');
      if (cur && cur.classList.contains("dimmed")) deselect();
    }
    if (level === "Avanc\u00e9" && available === 0) {
      selHint.textContent = "Aucun morceau \u00ab Avanc\u00e9 \u00bb pour l'instant"
                          + " \u2014 d'autres arrivent bient\u00f4t !";
    } else if (level === "all") {
      selHint.textContent = "Touche une note du clavier pour s\u00e9lectionner"
                          + " un morceau, puis lance l'apprentissage.";
    } else {
      selHint.textContent = available
        + " morceau" + (available > 1 ? "x" : "")
        + " \u00ab " + level + " \u00bb"
        + " disponible" + (available > 1 ? "s" : "")
        + " \u2014 touche une note allum\u00e9e.";
    }
  }

  document.getElementById("filters").addEventListener("click", function(e) {
    var p = e.target;
    while (p && p !== this) {
      if (p.classList.contains("pill")) { applyFilter(p.getAttribute("data-level")); return; }
      p = p.parentNode;
    }
  });

  // --- Toast ---
  var toastTimer = null;
  function showToast(msg) {
    toastMsg.textContent = msg;
    toastEl.classList.add("show");
    clearTimeout(toastTimer);
    toastTimer = setTimeout(function() { toastEl.classList.remove("show"); }, 2600);
  }

  // --- Toggle preview ---
  document.getElementById("previewSwitch").addEventListener("click", function() {
    previewOn = !previewOn;
    this.classList.toggle("on", previewOn);
    this.setAttribute("aria-checked", previewOn ? "true" : "false");
    httpGet("/preview?enabled=" + (previewOn ? 1 : 0));
  });

  // --- Requetes HTTP vers ESP32 (XHR + timeout) ---
  function httpGet(url) {
    var xhr = new XMLHttpRequest();
    xhr.timeout = 4000;
    xhr.open("GET", url, true);
    xhr.send();
  }

  // --- Lecture ---
  var TEMPO = 620;
  var curNote = 0, totalNotes = 0;

  function buildBar(n) {
    npBar.innerHTML = "";
    for (var i = 0; i < n; i++) {
      var s = document.createElement("div");
      s.className = "seg";
      npBar.appendChild(s);
    }
  }

  function paintBar() {
    var segs = npBar.children;
    for (var i = 0; i < segs.length; i++) {
      segs[i].className = "seg"
        + (i < curNote - 1 ? " done" : (i === curNote - 1 ? " cur" : ""));
    }
    npCur.textContent = curNote;
  }

  function tick() {
    if (curNote >= totalNotes) {
      clearInterval(progTimer); progTimer = null;
      app.classList.add("done");
      npCur.textContent = totalNotes;
      var segs = npBar.children;
      for (var i = 0; i < segs.length; i++) segs[i].className = "seg done";
      return;
    }
    curNote++;
    paintBar();
  }

  function play(songId) {
    playingSong  = songId;
    var song     = SONGS[songId];
    totalNotes   = song.notes;
    curNote      = 1;
    npTitle.textContent = song.name;
    npTotal.textContent = totalNotes;
    buildBar(totalNotes);
    paintBar();
    app.classList.remove("done");
    app.classList.add("playing");
    httpGet("/play?song=" + encodeURIComponent(songId) + "&preview=" + (previewOn ? 1 : 0));
    clearInterval(progTimer);
    progTimer = setInterval(tick, TEMPO);
  }

  function stop() {
    clearInterval(progTimer); progTimer = null;
    app.classList.remove("playing", "done");
    httpGet("/stop");
    playingSong = null;
  }

  document.getElementById("launchBtn").addEventListener("click", function() {
    if (selectedSong) play(selectedSong);
  });
  document.getElementById("stopBtn").addEventListener("click", stop);
  document.getElementById("restartBtn").addEventListener("click", function() {
    if (playingSong) play(playingSong);
  });

  // --- Init ---
  renderSelection();

})();
</script>
</body>
</html>)rawliteral";
