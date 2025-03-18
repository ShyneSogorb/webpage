import{r as f,a as p,b as E}from"./selector-engine.B3Qj5QDd.js";var e={exports:{}};/*!
  * Bootstrap component-functions.js v5.3.3 (https://getbootstrap.com/)
  * Copyright 2011-2024 The Bootstrap Authors (https://github.com/twbs/bootstrap/graphs/contributors)
  * Licensed under MIT (https://github.com/twbs/bootstrap/blob/main/LICENSE)
  */var b=e.exports,r;function x(){return r||(r=1,function(v,o){(function(t,n){n(o,f(),p(),E())})(b,function(t,n,a,c){const u=(i,l="hide")=>{const m=`click.dismiss${i.EVENT_KEY}`,s=i.NAME;n.on(document,m,`[data-bs-dismiss="${s}"]`,function(d){if(["A","AREA"].includes(this.tagName)&&d.preventDefault(),c.isDisabled(this))return;const g=a.getElementFromSelector(this)||this.closest(`.${s}`);i.getOrCreateInstance(g)[l]()})};t.enableDismissTrigger=u,Object.defineProperty(t,Symbol.toStringTag,{value:"Module"})})}(e,e.exports)),e.exports}export{x as r};
