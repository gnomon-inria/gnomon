// springy.js ---
//  This is a fork of the javascript springy to allow for
// making force directed graph layouts in qml
//

// Commentary:
//
//
//
//

// Change Log:
//
//
//

// Code:


.pragma library

.import QtQuick          as Q
.import QtQuick.Controls as Q

var Pipeline = {};

var Graph = Pipeline.Graph = function() {
	this.nodeSet = {};
	this.nodes = [];
	this.edges = [];
	this.adjacency = {};

	this.nextNodeId = 0;
	this.nextEdgeId = 0;
	this.eventListeners = [];
};

var Node = Pipeline.Node = function(id, data) {
	this.id = id;
	this.data = (data !== undefined) ? data : {};
    this.isSink = (data.outputEdgeCount !== undefined) ? (data.outputEdgeCount === 0 && data.inputEdgeCount > 0) : false
    this.isSource = (data.inputEdgeCount !== undefined) ? (data.outputEdgeCount > 0 && data.inputEdgeCount === 0) : false

	// Data fields used by layout algorithm in this file:
	// this.isSink
	// this.data.isSource
};

var Edge = Pipeline.Edge = function(id, source, target, data) {
	this.id = id;
	this.source = source;
	this.target = target;
	this.data = (data !== undefined) ? data : {};

	// Edge data field used by layout alorithm
	// this.data.length
	// this.data.type
};

Graph.prototype.addNode = function(node) {
	if (!(node.id in this.nodeSet)) {
		this.nodes.push(node);
	}

	this.nodeSet[node.id] = node;

	this.notify();
	return node;
};

Graph.prototype.addNodes = function() {
	// accepts variable number of arguments, where each argument
	// is a string that becomes both node identifier and label
	for (var i = 0; i < arguments.length; i++) {
		var name = arguments[i];
		var node = new Node(name, {label:name});
		this.addNode(node);
	}
};

Graph.prototype.addEdge = function(edge) {
	var exists = false;
	this.edges.forEach(function(e) {
		if (edge.id === e.id) { exists = true; }
	});

	if (!exists) {
		this.edges.push(edge);
	}

	if (!(edge.source.id in this.adjacency)) {
		this.adjacency[edge.source.id] = {};
	}
	if (!(edge.target.id in this.adjacency[edge.source.id])) {
		this.adjacency[edge.source.id][edge.target.id] = [];
	}

	exists = false;
	this.adjacency[edge.source.id][edge.target.id].forEach(function(e) {
		if (edge.id === e.id) { exists = true; }
	});

	if (!exists) {
		this.adjacency[edge.source.id][edge.target.id].push(edge);
	}

	this.notify();
	return edge;
};

Graph.prototype.addEdges = function() {
	// accepts variable number of arguments, where each argument
	// is a triple [nodeid1, nodeid2, attributes]
	for (var i = 0; i < arguments.length; i++) {
		var e = arguments[i];
		var node1 = this.nodeSet[e[0]];
		if (node1 == undefined) {
			throw new TypeError("invalid node name: " + e[0]);
		}
		var node2 = this.nodeSet[e[1]];
		if (node2 == undefined) {
			throw new TypeError("invalid node name: " + e[1]);
		}
		var attr = e[2];

		this.newEdge(node1, node2, attr);
	}
};

Graph.prototype.newNode = function(data) {
	var node = new Node(this.nextNodeId++, data);
	this.addNode(node);
	return node;
};

Graph.prototype.newEdge = function(source, target, data) {
	var edge = new Edge(this.nextEdgeId++, source, target, data);
	this.addEdge(edge);
	return edge;
};


// add nodes and edges from JSON object
Graph.prototype.loadJSON = function(json) {
	/**
	   Pipeline's simple JSON format for graphs.

	   historically, Pipeline uses separate lists
	   of nodes and edges:

	   {
	   "nodes": [
	   "center",
	   "left",
	   "right",
	   "up",
	   "satellite"
	   ],
	   "edges": [
	   ["center", "left"],
	   ["center", "right"],
	   ["center", "up"]
	   ]
	   }

	**/
	// parse if a string is passed (EC5+ browsers)
	if (typeof json == 'string' || json instanceof String) {
		json = JSON.parse( json );
	}

	if ('nodes' in json || 'edges' in json) {
		this.addNodes.apply(this, json['nodes']);
		this.addEdges.apply(this, json['edges']);
	}
}


// find the edges from node1 to node2
Graph.prototype.getEdges = function(node1, node2) {
	if (node1.id in this.adjacency
		&& node2.id in this.adjacency[node1.id]) {
		return this.adjacency[node1.id][node2.id];
	}

	return [];
};

// remove a node and it's associated edges from the graph
Graph.prototype.removeNode = function(node) {
	if (node.id in this.nodeSet) {
		delete this.nodeSet[node.id];
	}

	for (var i = this.nodes.length - 1; i >= 0; i--) {
		if (this.nodes[i].id === node.id) {
			this.nodes.splice(i, 1);
		}
	}

	this.detachNode(node);
};

// removes edges associated with a given node
Graph.prototype.detachNode = function(node) {
	var tmpEdges = this.edges.slice();
	tmpEdges.forEach(function(e) {
		if (e.source.id === node.id || e.target.id === node.id) {
			this.removeEdge(e);
		}
	}, this);

	this.notify();
};

// remove a node and it's associated edges from the graph
Graph.prototype.removeEdge = function(edge) {
	for (var i = this.edges.length - 1; i >= 0; i--) {
		if (this.edges[i].id === edge.id) {
			this.edges.splice(i, 1);
		}
	}

	for (var x in this.adjacency) {
		for (var y in this.adjacency[x]) {
			var edges = this.adjacency[x][y];

			for (var j=edges.length - 1; j>=0; j--) {
				if (this.adjacency[x][y][j].id === edge.id) {
					this.adjacency[x][y].splice(j, 1);
				}
			}

			// Clean up empty edge arrays
			if (this.adjacency[x][y].length == 0) {
				delete this.adjacency[x][y];
			}
		}

		// Clean up empty objects
		if (isEmpty(this.adjacency[x])) {
			delete this.adjacency[x];
		}
	}

	this.notify();
};

/* Merge a list of nodes and edges into the current graph. eg.
  var o = {
  nodes: [
  {id: 123, data: {type: 'user', userid: 123, displayname: 'aaa'}},
  {id: 234, data: {type: 'user', userid: 234, displayname: 'bbb'}}
  ],
  edges: [
  {from: 0, to: 1, type: 'submitted_design', directed: true, data: {weight: }}
  ]
  }
*/
Graph.prototype.merge = function(data) {
	var nodes = [];
	data.nodes.forEach(function(n) {
		nodes.push(this.addNode(new Node(n.id, n.data)));
	}, this);

	data.edges.forEach(function(e) {
		var from = nodes[e.from];
		var to = nodes[e.to];

		var id = (e.directed)
			? (id = e.type + "-" + from.id + "-" + to.id)
			: (from.id < to.id) // normalise id for non-directed edges
			? e.type + "-" + from.id + "-" + to.id
			: e.type + "-" + to.id + "-" + from.id;

		var edge = this.addEdge(new Edge(id, from, to, e.data));
		edge.data.type = e.type;
	}, this);
};

Graph.prototype.filterNodes = function(fn) {
	var tmpNodes = this.nodes.slice();
	tmpNodes.forEach(function(n) {
		if (!fn(n)) {
			this.removeNode(n);
		}
	}, this);
};

Graph.prototype.filterEdges = function(fn) {
	var tmpEdges = this.edges.slice();
	tmpEdges.forEach(function(e) {
		if (!fn(e)) {
			this.removeEdge(e);
		}
	}, this);
};


Graph.prototype.addGraphListener = function(obj) {
	this.eventListeners.push(obj);
};

Graph.prototype.notify = function() {
	this.eventListeners.forEach(function(obj){
		obj.graphChanged();
	});
};

// /////////////////////////////////////////////////////////////////////////////
// LAYOUT
// /////////////////////////////////////////////////////////////////////////////

var Layout = Pipeline.Layout = {};
Layout.ForceDirected = function(graph, stiffness, repulsion, damping, minEnergyThreshold, maxSpeed) {
    this.graph = graph;
    this.iterations = 1000;
    this.target_distance = 200;
    this.target_radius = 200;
    this.max_deformation = 5;

    this.node_distances = {};
    this.node_vectors = {};

	this.nodePoints = {}; // keep track of points associated with nodes
	this.edgeSprings = {}; // keep track of springs associated with edges
};

Layout.ForceDirected.prototype.point = function(node) {
	if (!(node.id in this.nodePoints)) {
		this.nodePoints[node.id] = new Layout.ForceDirected.Point(Vector.random());
	}

	return this.nodePoints[node.id];
};

Layout.ForceDirected.prototype.spring = function(edge) {
	if (!(edge.id in this.edgeSprings)) {

		var existingSpring = false;

		var from = this.graph.getEdges(edge.source, edge.target);
		from.forEach(function(e) {
			if (existingSpring === false && e.id in this.edgeSprings) {
				existingSpring = this.edgeSprings[e.id];
			}
		}, this);

		if (existingSpring !== false) {
			return new Layout.ForceDirected.Spring(existingSpring.point1, existingSpring.point2);
		}

		var to = this.graph.getEdges(edge.target, edge.source);
		from.forEach(function(e){
			if (existingSpring === false && e.id in this.edgeSprings) {
				existingSpring = this.edgeSprings[e.id];
			}
		}, this);

		if (existingSpring !== false) {
			return new Layout.ForceDirected.Spring(existingSpring.point2, existingSpring.point1);
		}

		this.edgeSprings[edge.id] = new Layout.ForceDirected.Spring(
			this.point(edge.source), this.point(edge.target)
		);
	}

	return this.edgeSprings[edge.id];
};

// callback should accept two arguments: Node, Point
Layout.ForceDirected.prototype.eachNode = function(callback) {
	var t = this;
	this.graph.nodes.forEach(function(n){
		callback.call(t, n, t.point(n));
	});
};

// callback should accept two arguments: Edge, Spring
Layout.ForceDirected.prototype.eachEdge = function(callback) {
	var t = this;
	this.graph.edges.forEach(function(e){
		callback.call(t, e, t.spring(e));
	});
};

// callback should accept one argument: Spring
Layout.ForceDirected.prototype.eachSpring = function(callback) {
	var t = this;
	this.graph.edges.forEach(function(e){
		callback.call(t, t.spring(e));
	});
};

Layout.ForceDirected.prototype.run = function () {

    //calculate the target radius
    this.target_radius = this.target_distance*Math.pow(this.graph.nodes.length, 0.5) + 0.00000001;


    //debug
    this.eachNode(function(node, point){
        console.log("node: " + node.id + " isSink: " + node.isSink
                    + " isSource: " + node.isSource + " force: (" + point.f.x + ", " + point.f.y
                    + ") initialPosition: (" + point.p.x + ", " + point.p.y + ")")
    })

    //run the computation of the layout for the given number of iterations
    for(let i = 0; i < this.iterations; i++) {

        //run the calculations
        this.step()

        //update the positions
	    this.updatePosition()
    }

    //center all the points
    this.center()

    //debug
    this.eachNode(function(node, point){
        console.log("node: " + node.id + " finalforce: (" + point.f.x + ", " + point.f.y
                    + ") finalPosition: (" + point.p.x + ", " + point.p.y + ")"
                    + " GUILLAUME VERSION: (" + node.data.position.x + ", " + node.data.position.y + ")")
    })

}

Layout.ForceDirected.prototype.step = function () {

    //first reinitialize the forces
    this.eachNode(function(node, point){
        point.f = new Vector(0, 0)
    })

    //And recompute them
    this.computeRepulsion()
    this.computeAttraction()
    this.computeLeftDrift()
    this.computeRightDrift()
    this.centerVertically()
    this.horizontalize()

}

Layout.ForceDirected.prototype.computeRepulsion = function() {

	this.eachNode(function(n1, point1) {
		this.eachNode(function(n2, point2) {
			if (point1 !== point2)
			{
				const d = point1.p.subtract(point2.p);
				const distance = d.magnitude() + 0.1; // avoid massive forces at small distances (and divide by zero)
				const direction = d.normalise();
                const weight = 1.0

                const repulsion = direction.multiply(Math.pow(this.target_distance, 2)).divide(Math.pow(distance, 2) + 0.0000001).multiply(weight);

				point1.applyForce(repulsion);
				point2.applyForce(repulsion);
			}
		});
    })

}

Layout.ForceDirected.prototype.computeAttraction = function() {

    this.eachSpring(function(spring){

        const d1 = spring.point2.p.subtract(spring.point1.p);
        const d2 = spring.point1.p.subtract(spring.point2.p);
		const distance = d1.magnitude()+ 0.1; // avoid massive forces at small distances (and divide by zero)
		const direction1 = d1.normalise();
		const direction2 = d2.normalise();
        const weight = 1.0
        const attraction1 = direction1.multiply(this.target_distance - distance).divide(this.target_distance).multiply(weight);
        const attraction2 = direction2.multiply(this.target_distance - distance).divide(this.target_distance).multiply(weight);

		// apply force to each end point
		spring.point1.applyForce(attraction2);
		spring.point2.applyForce(attraction1);

	});
    
}

Layout.ForceDirected.prototype.computeLeftDrift = function() {

    const weight = 0.5

	this.eachNode(function(node, point) {
        if(node.isSource) {
            const x_drift = - this.target_radius - point.p.x
            if(x_drift < 0) {
                const force = new Vector((x_drift * Math.abs(x_drift) / Math.pow(this.target_distance, 2)), 0).multiply(weight)
                point.applyForce(force)
            }
        }
	})

}

Layout.ForceDirected.prototype.computeRightDrift = function() {

    const weight = 0.5
	this.eachNode(function(node, point) {
        if(node.isSink) {
            const x_drift = this.target_radius - point.p.x
            if(x_drift > 0) {
                const force = new Vector((x_drift * Math.abs(x_drift) / Math.pow(this.target_distance, 2)), 0).multiply(weight)
                point.applyForce(force)
            }
        }
	})

}

Layout.ForceDirected.prototype.centerVertically = function() {

    const weight = 1.0
	this.eachNode(function(node, point) {
        const force = new Vector(0, (point.p.y * Math.abs(point.p.y) / Math.pow(this.target_radius, 2)))
        point.applyForce(force.multiply(weight))
	})

}

Layout.ForceDirected.prototype.horizontalize = function() {

    const weight = 1.0
    this.eachSpring(function(spring){

        const d = spring.point2.p.subtract(spring.point1.p); // the direction of the spring
		const distance = d.magnitude()+ 0.1; // avoid massive forces at small distances (and divide by zero)
		const direction = d.normalise();
        const edge_sinus = direction.y / distance
        const force1 = Vector(-Math.abs(edge_sinus), -edge_sinus)
        const force2 = Vector(Math.abs(edge_sinus), edge_sinus)

		// apply force to each end point
		spring.point1.applyForce(force1.multiply(weight));
		spring.point2.applyForce(force2.multiply(weight));

	});

}

Layout.ForceDirected.prototype.center = function() {
    let center = new Vector(0, 0)
    this.graph.nodes.map(node => {
        const point = this.point(node).p
        center = center.add(point)
    })

    center.divide(this.graph.nodes.length)
    this.eachNode(function (node, point) {
        point.p = point.p.subtract(center)
    })
}

Layout.ForceDirected.prototype.updatePosition = function() {
	this.eachNode(function(node, point) {
		// Same question as above; along with updateVelocity, is this all of
		// your integration code?

		const length = point.f.magnitude()
        if(length > this.max_deformation) point.f = point.f.multiply(this.max_deformation / length)

        point.p = point.p.add(point.f);
	});
};

// Find the nearest point to a particular position
Layout.ForceDirected.prototype.nearest = function(pos) {
	var min = {node: null, point: null, distance: null};
	var t = this;
	this.graph.nodes.forEach(function(n){
		var point = t.point(n);
		var distance = point.p.subtract(pos).magnitude();

		if (min.distance === null || distance < min.distance) {
			min = {node: n, point: point, distance: distance};
		}
	});

	return min;
};

// returns [topleft, bottomright] (to follow qml specs)
Layout.ForceDirected.prototype.getBoundingBox = function() {
	var topleft = new Vector(-2,2);
	var bottomright = new Vector(2,-2);

	this.eachNode(function(n, point) {
		if (point.p.x < topleft.x) {
			topleft.x = point.p.x;
		}
		if (point.p.y > topleft.y) {
			topleft.y = point.p.y;
		}
		if (point.p.x > bottomright.x) {
			bottomright.x = point.p.x;
		}
		if (point.p.y < bottomright.y) {
			bottomright.y = point.p.y;
		}
	});

	var padding = bottomright.subtract(topleft).multiply(0.07); // ~5% padding

	return {topleft: topleft.subtract(padding), bottomright: bottomright.add(padding)};
};


// Vector
var Vector = Pipeline.Vector = function(x, y) {
	this.x = x;
	this.y = y;
};

Vector.random = function() {
	return new Vector(Math.random(), Math.random());
};

Vector.prototype.add = function(v2) {
	return new Vector(this.x + v2.x, this.y + v2.y);
};

Vector.prototype.subtract = function(v2) {
	return new Vector(this.x - v2.x, this.y - v2.y);
};

Vector.prototype.multiply = function(n) {
	return new Vector(this.x * n, this.y * n);
};

Vector.prototype.divide = function(n) {
	return new Vector((this.x / n) || 0, (this.y / n) || 0); // Avoid divide by zero errors..
};

Vector.prototype.magnitude = function() {
	return Math.sqrt(this.x*this.x + this.y*this.y);
};

Vector.prototype.normal = function() {
	return new Vector(-this.y, this.x);
};

Vector.prototype.normalise = function() {
	return this.divide(this.magnitude() + 0.00000001);
};

// Point
Layout.ForceDirected.Point = function(position) {
	this.p = position; // position
	this.f = new Vector(0, 0); // velocity
};

Layout.ForceDirected.Point.prototype.applyForce = function(force) {
	this.f = this.f.add(force);
};

// Spring
Layout.ForceDirected.Spring = function(point1, point2) {
	this.point1 = point1;
	this.point2 = point2;
};

var isEmpty = function(obj) {
	for (var k in obj) {
		if (obj.hasOwnProperty(k)) {
			return false;
		}
	}
	return true;
};

//
// springy.js ends here
