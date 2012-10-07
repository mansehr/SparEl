var MAX_WATTS = 12000;
var NUM_WATT_LINES = 12;
var NUM_SAMPLES = 50;
var WIDTH = 400;
var HEIGHT = 250;
var X = 300;
var Y = 0;
var MARGIN = 60;
var DEBUG = false;

var path;
var paper = Raphael(X, Y, WIDTH+2*MARGIN, HEIGHT+2*MARGIN);
var graphValues = [];

var graphInit = function() {
  // y-axis
  var increment = MAX_WATTS / NUM_WATT_LINES;
  for (var i = 0; i <= NUM_WATT_LINES; i++) {
    // line
    var y = yForWatt(i * increment);
    paper.path(['M', xForSample(0), y, 'L', xForSample(NUM_SAMPLES-1), y])
      .attr("stroke", "#ccc");
    // text
    paper.text(MARGIN - 30, y, (i*increment) + " W");
  }

  // diagram boundry
  paper.rect(xForSample(0), yForWatt(MAX_WATTS), WIDTH, HEIGHT);

  // canvas boundry
  if (DEBUG) {
    paper.rect(0, 0, WIDTH+2*MARGIN, HEIGHT+2*MARGIN);
  }
};

var graphAdd = function(time, value) {
  graphValues.push({time: time, value: value});
  while (graphValues.length > NUM_SAMPLES) {
    graphValues.splice(0, 1);
  }

  if (path) {
    path.remove();
  }
  var pathStr = "M";
  for (var i=0; i<graphValues.length; i++) {
    pathStr += xForSample(i);
    pathStr += " ";
    pathStr += yForWatt(graphValues[i].value);
    if (i < graphValues.length - 1) {
      pathStr += "L";
    }
  }
  path = paper.path(pathStr)
      .attr("stroke", "#c00")
      .attr("stroke-width", "2");
};

var xForSample = function(i) {
  return MARGIN + i * (WIDTH / (NUM_SAMPLES-1));
}

var yForWatt = function(watt) {
  return MARGIN + HEIGHT - (watt / MAX_WATTS) * HEIGHT;
}
