var START_VERTICES = 2048;

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

    /* Background canvas. */
    var bg_canvas = document.createElement('canvas');
    var bg_context = bg_canvas.getContext('2d');
    var bg_image = new Image;
    bg_image.onload = function() {
        bg_context.drawImage(bg_image, 0, 0, bg_canvas.width, bg_canvas.height);
    };
    bg_image.src = 'background.jpg';

    /* Set up display. */
    display = new Display(canvas);
    display.clear();
    for (var i = 0; i < START_VERTICES; i++) {
        display.add(new Point());
    }
    display.setBackground(bg_canvas, bg_context);
    display.draw();

    /* Set up controller. */
    controller = new Controller(display, canvas);
});

window.addEventListener('mousewheel', function(event) {
    event.preventDefault();
    return false;
}, false);
