// hamburger menu
const hamburgerMenu = document.querySelector("#navigation .nav-icon");
const navContent = document.querySelector("#nav-content");
const closeNavButton = document.querySelector("#nav-content .close-btn");

hamburgerMenu.addEventListener("click", () => {
    navContent.classList.add("show");
    document.body.style.overflow = "hidden";
});

closeNavButton.addEventListener("click", () => {
    navContent.classList.remove("show");
    document.body.style.overflow = "initial";
});


// auto scroll
const smoothScrollTrigger = document.querySelectorAll('a[href^="#"]');
for (let i = 0; i < smoothScrollTrigger.length; i++){
  smoothScrollTrigger[i].addEventListener('click', (e) => {
    e.preventDefault();
    let href = smoothScrollTrigger[i].getAttribute('href');
    let targetElement = document.getElementById(href.replace('#', ''));
    const rect = targetElement.getBoundingClientRect().top;
    const offset = window.pageYOffset;
    const target = rect + offset;
    window.scrollTo({
      top: target,
      behavior: "smooth",
    });
    navContent.classList.remove("show");
    document.body.style.overflow = "initial";
  });
}


// wave effect
var unit = 100, canvasList, info = {}, colorList;
function init() {
  info.seconds = 0;
  info.t = 0;
	canvasList = [];
  colorList = [];
  // first canvas
  canvasList.push(document.getElementById("waveCanvas"));
  colorList.push(['#666', '#ccc', '#eee']);
  // second canvas
  canvasList.push(document.getElementById("waveCanvas2"));
  colorList.push(['#43c0e4']);
  // third canvas
  canvasList.push(document.getElementById("waveCanvas3"));
  colorList.push(['#fff']);
	for(var canvasIndex in canvasList) {
    var canvas = canvasList[canvasIndex];
    canvas.width = document.documentElement.clientWidth; 
    canvas.height = 500;
    canvas.contextCache = canvas.getContext("2d");
  }
	update();
}
function update() {
	for(var canvasIndex in canvasList) {
    var canvas = canvasList[canvasIndex];
    draw(canvas, colorList[canvasIndex]);
  }
  info.seconds = info.seconds + .014;
  info.t = info.seconds*Math.PI;
  setTimeout(update, 45);
}
function draw(canvas, color) {
  var context = canvas.contextCache;
  context.clearRect(0, 0, canvas.width, canvas.height);
  drawWave(canvas, color[0], 0.5, 3, 0);
	drawWave(canvas, color[1], 0.4, 2, 250);
	drawWave(canvas, color[2], 0.2, 1.6, 100);
}
function drawWave(canvas, color, alpha, zoom, delay) {
	var context = canvas.contextCache;
  context.fillStyle = color;
  context.globalAlpha = alpha;
  context.beginPath();
  drawSine(canvas, info.t / 0.5, zoom, delay);
  context.lineTo(canvas.width + 10, canvas.height); 
  context.lineTo(0, canvas.height);
  context.closePath();
  context.fill(); 
}
function drawSine(canvas, t, zoom, delay) {
  var xAxis = Math.floor(canvas.height/2);
  var yAxis = 0;
  var context = canvas.contextCache;
  var x = t;
  var y = Math.sin(x)/zoom;
  context.moveTo(yAxis, unit*y+xAxis);
  for (i = yAxis; i <= canvas.width + 10; i += 10) {
      x = t+(-yAxis+i)/unit/zoom;
      y = Math.sin(x - delay)/3;
      context.lineTo(i, unit*y+xAxis);
  }
}

init();
