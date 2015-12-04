function Display(canvas) {
    var display, gl = Igloo.getContext(canvas);
    if (gl == null) {
        console.error('WebGL initialization failed!');
        display = new Display2D(canvas.getContext('2d'));
        document.title = document.title.replace(/WebGL/, 'HTML5 Canvas');
    } else {
        display = new DisplayGL2(gl);
    }
    display.points = [];
    display.selection = null;
    display.canvas = canvas;
    return display;
}

Display.prototype.max = 4096;

Display.prototype.add = function(point) {
    if (this.points.length < this.max) {
        this.points.push(point);
    }
    return this;
};

Display.prototype.remove = function(point) {
    var ps = this.points;
    for (var i = 0; i < ps.length; i++) {
        if (ps[i] === point) {
            if (i === ps.length - 1) {
                ps.pop();
            } else {
                ps[i] = ps.pop();
            }
            break;
        }
    }
    return this;
};

Display.prototype.cursor = function(name) {
    this.canvas.style.cursor = name;
    return this;
};

Display.prototype.select = function(point) {
    if (this.selection !== point) {
        this.selection = point;
        this.draw();
    }
    return this;
};

Display.prototype.setBackground = function(bg_canvas, bg_context) {
    this.bg_canvas = bg_canvas;
    this.bg_context = bg_context;
};
