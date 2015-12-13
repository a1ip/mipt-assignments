var START_VERTICES = 2;
var START_POLYLINES = 2;

function $(s) {
    return document.querySelector(s);
}

var display = null, controller = null;
window.addEventListener('load', function() {
    /* Update cavas size. */
    var canvas = $('#voronoi'),
        parent = canvas.parentNode;
    canvas.width = parent.offsetWidth;
    canvas.height = parent.offsetHeight;

    /* Set up display. */
    display = new Display(canvas);
    display.clear();

    for (var k = 0; k < START_POLYLINES; k++) {
        var new_polyline = PL();
        display.polylines.push(new_polyline);
        for (var i = 0; i < START_VERTICES; i++) {
            var newPoint = new Point(null, null, new_polyline.r,
                new_polyline.g, new_polyline.b, new_polyline);
            display.add(newPoint);
            new_polyline.add(newPoint);
        }
    }
    display.draw();

    /* Set up controller. */
    controller = new Controller(display, canvas);
});

window.addEventListener('mousewheel', function(event) {
    event.preventDefault();
    return false;
}, false);
