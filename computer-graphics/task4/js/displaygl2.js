function DisplayGL2(gl) {
    this.gl = gl;

    this.resolution = 63;

    this.alpha_tent = 1.0;
    this.alpha_cone = 1.0;

    /* Prepare cone model. */
    var w = this.gl.canvas.width,
        h = this.gl.canvas.height,
        D = vec2(w, h).magnitude(),
        a = vec2(w, h).fdivide(D);
    this.cone_rad = 2.5;

    var cone = [0.0, 0.0, 0.0];
    for (var i = 0; i < this.resolution; i++) {
        var v = i / (this.resolution - 1) * Math.PI * 2;
        cone.push(Math.cos(v) * a.y);
        cone.push(Math.sin(v) * a.x);
        cone.push(1.0);
    }

    var tent_base = [
        0, 0, 0,   0, 1, 1,   1, 1, 1,
        0, 0, 0,   1, 0, 0,   1, 1, 1,
        0, 0, 0,   0,-1, 1,   1,-1, 1,
        0, 0, 0,   1, 0, 0,   1,-1, 1
    ];

    this.instancing = gl.getExtension("ANGLE_instanced_arrays");
    var vert, frag;
    if (this.instancing == null) {
        document.title += ' (no instancing)';
        vert = 'glsl/cone.vert';
        frag = 'glsl/cone.frag';
    } else {
        vert = 'glsl/cone-instanced.vert';
        frag = 'glsl/cone-instanced.frag';
    }

    this.programs = {
        cone: new Igloo.Program(gl, vert, frag),
        points: new Igloo.Program(gl, 'glsl/point.vert', 'glsl/point.frag'),
        chosen_point: new Igloo.Program(gl, 'glsl/chosen_point.vert', 'glsl/chosen_point.frag'),
        tents: new Igloo.Program(gl, 'glsl/tent.vert', 'glsl/tent.frag'),
        polylines: new Igloo.Program(gl, 'glsl/polyline.vert', 'glsl/polyline.frag'),
    };

    this.buffers = {
        cone: new Igloo.Buffer(gl, new Float32Array(cone)),
        tent_base: new Igloo.Buffer(gl, new Float32Array(tent_base)),
        points: new Igloo.Buffer(gl, null, gl.STREAM_DRAW),
        wq: new Igloo.Buffer(gl, null, gl.STREAM_DRAW),
        chosen_point: new Igloo.Buffer(gl, null, gl.STREAM_DRAW),
        dark: new Igloo.Buffer(gl, null, gl.STREAM_DRAW),
        colors: new Igloo.Buffer(gl, null, gl.STREAM_DRAW),
        t_points: new Igloo.Buffer(gl, null, gl.STREAM_DRAW),
        t_pointsch: new Igloo.Buffer(gl, null, gl.STREAM_DRAW)
    };

    gl.enable(gl.BLEND);
    gl.enable(gl.DEPTH_TEST);
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA);
}

DisplayGL2.prototype = Object.create(Display.prototype);
DisplayGL2.prototype.constructor = DisplayGL2;

DisplayGL2.prototype.update_cone = function() {
    var w = this.gl.canvas.width, h = this.gl.canvas.height,
        a = vec2(w, h).fdivide(vec2(w, h).magnitude());
    var cone;
    if (this.space > 1) {
        cone = [0.0, 0.0, 0.0];
        this.resolution = 63;
        for (var i = 0; i < this.resolution; i++) {
            var v = i / (this.resolution - 1) * Math.PI * 2;
            var Ro = Math.pow(Math.pow(Math.abs(Math.cos(v)), this.space) +
                  Math.pow(Math.abs(Math.sin(v)), this.space), 1. / this.space);
            cone.push(Math.cos(v) * a.y / Ro);
            cone.push(Math.sin(v) * a.x / Ro);
            cone.push(1.0);
        }
    } else {
        this.resolution = 5;
        if (this.space == 1) {
            cone = [
                0.0, 0.0, 0.0,
                -a.y, 0, 1.0,
                0, a.x, 1.0,
                a.y, 0, 1.0,
                0, -a.x, 1.0,
                -a.y, 0, 1.0
            ];
        } else {
            cone = [
                0.0, 0.0, 0.0,
                -a.y, -a.x, 1.0,
                -a.y, a.x, 1.0,
                a.y, a.x, 1.0,
                a.y, -a.x, 1.0,
                -a.y, -a.x, 1.0
            ];
        }
    }
    this.buffers.cone.update(new Float32Array(cone));
}

DisplayGL2.prototype.switchNorm = function() {
    this.space = (this.space + 1) % 3;
    this.update_cone();
    this.draw();
}

DisplayGL2.prototype.changeRadius = function(diff) {
    this.cone_rad += diff;
    this.update_cone();
    this.draw();
}

DisplayGL2.prototype.changeWeight = function(dw, dq) {
    if (this.chosen == null)
        return;
    var polyline = this.chosen.parent;
    polyline.w = 1.0 / (1.0 / polyline.w + dw);
    polyline.q += dq;
    this.draw();
}

DisplayGL2.prototype.clear = function() {
    var gl = this.gl;
    gl.clearColor(0.5, 0.5, 0.5, 1.0);
    gl.clear(this.gl.COLOR_BUFFER_BIT);
    gl.clear(this.gl.DEPTH_BUFFER_BIT);
    return this;
};

DisplayGL2.prototype.draw = function() {
    var gl = this.gl, ext = this.instancing;
    gl.clear(this.gl.DEPTH_BUFFER_BIT);

    var dark = [], points = [], colors = [], t_points = [], t_pointsch = [], wq = [];
    this.points.forEach(function(p) {
        points.push(p.x);
        points.push(1 - p.y);
        wq.push(p.parent.w);
        wq.push(p.parent.q);
        colors.push(p.r / 255);
        colors.push(p.g / 255);
        colors.push(p.b / 255);
        var drk = (p.isDark() ? 1 : 0);
        if (p == this.chosen) drk = !drk;
        dark.push(drk);
    });

    this.buffers.points.update(new Float32Array(points));
    this.buffers.wq.update(new Float32Array(wq));
    this.buffers.dark.update(new Float32Array(dark));
    this.buffers.colors.update(new Float32Array(colors));
    var cone = this.programs.cone.use();
    cone.uniform('alpha', this.alpha_cone)
    .uniform('conerad', this.cone_rad);
    if (ext != null) {
        /* Use instancing extension. */
        cone.attrib('vert', this.buffers.cone, 3)
            .attrib('position', this.buffers.points, 2)
            .attrib('wq', this.buffers.wq, 2)
            .attrib('color', this.buffers.colors, 3);
        ext.vertexAttribDivisorANGLE(cone.vars['vert'], 0);
        ext.vertexAttribDivisorANGLE(cone.vars['position'], 1);
        ext.vertexAttribDivisorANGLE(cone.vars['wq'], 1);
        ext.vertexAttribDivisorANGLE(cone.vars['color'], 1);

        ext.drawArraysInstancedANGLE(gl.TRIANGLE_FAN, 0, this.resolution + 1,
                                     this.points.length);
    } else {
        /* One draw per Voronoi vertex. */
        cone.attrib('vert', this.buffers.cone, 3);
        for (var i = 0; i < this.points.length; i++) {
            var color = vec3(colors[i * 3 + 0],colors[i * 3 + 1],colors[i * 3 + 2]),
                wq1 = vec2(wq[i * 2 + 0],wq[i * 2 + 1]),
                position = vec2(points[i * 2 + 0], points[i * 2 + 1]);
            cone.uniform('color', color)
                .uniform('position', position)
                .uniform('wq', wq1)
                .draw(gl.TRIANGLE_FAN, this.resolution + 1);
        }
    }

    var tent = this.programs.tents.use();
    tent.attrib('vert', this.buffers.tent_base, 3)
        .uniform('conerad', this.cone_rad)
        .uniform('alpha', this.alpha_tent);
    var w = this.gl.canvas.width, h = this.gl.canvas.height,
        a = vec2(w, h).fdivide(vec2(w, h).magnitude());

    for (var k = 0; k < this.polylines.length; k++) {
        var l = this.polylines[k];
        var ch = 0;
        if ((this.chosen != null) && (this.chosen.parent == l)) ch = 1;
        if (l.points.length > 1) {
            var wq = vec2(l.w, l.q);
            var color = vec3(l.r / 255, l.g / 255, l.b / 255);
            tent.uniform('m_color', color)
                .uniform('wq', wq)
            for (var i = 1; i < l.points.length; i++) {
                prev_p = l.points[i - 1];
                p = l.points[i];
                var position = vec2(p.x, 1 - p.y);
                var prevposition = vec2(prev_p.x, 1 - prev_p.y);
                t_points.push(prevposition.x);
                t_points.push(prevposition.y);
                t_points.push(position.x);
                t_points.push(position.y);
                if (ch) {
                    t_pointsch.push(prevposition.x);
                    t_pointsch.push(prevposition.y);
                    t_pointsch.push(position.x);
                    t_pointsch.push(position.y);
                }

                var delta_x = vec2((prevposition.x - position.x) * 2.0, (prevposition.y - position.y) * 2.0);
                var dx = delta_x.x * a.x;
                var dy = delta_x.y * a.y;
                var len = Math.sqrt(dx * dx + dy * dy);
                var cosa = dx / len;
                var sina = dy / len;
                var Ro;
                var delta_y;
                if (this.space > 1) {
                    if (cosa == 0.0 || sina == 0.0 || this.space == 2.0) {
                        delta_y = vec2(-sina * a.y * this.cone_rad, cosa * a.x * this.cone_rad);
                    }
                    else {
                        var cosfi, sinfi;
                        var tga = sina / cosa;
                        var tgfi = Math.pow(Math.abs(tga), 1 / (1 - this.space));
                        cosfi = 1 / Math.sqrt(1 + tgfi * tgfi);
                        sinfi = tgfi * cosfi;
                        cosfi = cosfi * -Math.sign(sina);
                        sinfi = sinfi * Math.sign(cosa);
                        Ro = Math.pow(Math.pow(Math.abs(cosfi), this.space) +
                            Math.pow(Math.abs(sinfi), this.space), 1. / this.space);
                        delta_y = vec2(cosfi * a.y * this.cone_rad / Ro, sinfi * a.x * this.cone_rad / Ro);
                    }
                }
                else {
                    if (this.space == 0)
                        delta_y = vec2(-Math.sign(sina) * a.y * this.cone_rad,
                            Math.sign(cosa) * a.x * this.cone_rad);
                    if (this.space == 1) {
                        if (Math.abs(cosa) > Math.abs(sina)) delta_y = vec2(0.0, a.x * this.cone_rad);
                        else delta_y = vec2(a.y * this.cone_rad, 0.0);
                    }
                }

                tent
                .uniform('position', position)
                .uniform('delta_x', delta_x)
                .uniform('delta_y', delta_y)
                .draw(gl.TRIANGLES, 12);
            }
        }
    }
    this.buffers.t_points.update(new Float32Array(t_points));
    this.buffers.t_pointsch.update(new Float32Array(t_pointsch));

    /* Draw points. */
    var selection;
    if (this.selection != null) {
        selection = vec2(this.selection.x, 1 - this.selection.y);
    } else {
        selection = vec2(NaN, NaN); // matches nothing
    }
    var ch_p = [];
    if (this.chosen != null) {
        ch_p.push(this.chosen.x);
        ch_p.push(1 - this.chosen.y);
        this.buffers.chosen_point.update(new Float32Array(ch_p));
        var ch_col = vec3(1.0 - this.chosen.r / 255, 1.0 - this.chosen.g / 255, 1.0 - this.chosen.b / 255);
        this.programs.chosen_point.use()
        .uniform('color', ch_col)
        .attrib('position', this.buffers.chosen_point, 2)
        .draw(gl.POINTS, 1);

        this.programs.polylines.use()
        .uniform('color', ch_col)
        .attrib('position', this.buffers.t_pointsch, 2)
        .draw(gl.LINES, t_pointsch.length / 2);
    }

    polyline_def_color = vec3(0.0, 0.0, 0.0);
    this.programs.polylines.use()
        .uniform('color', polyline_def_color)
        .attrib('position', this.buffers.t_points, 2)
        .draw(gl.LINES, t_points.length / 2);

    this.programs.points.use()
        .attrib('position', this.buffers.points, 2)
        .attrib('dark', this.buffers.dark, 1)
        .uniform('selection', selection)
        .draw(gl.POINTS, this.points.length);

    return this;
};
