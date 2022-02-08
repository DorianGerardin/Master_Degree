/*var my_chart = null;
var simple_chart_config = null;
var firstNodeLabel = "<3-3-"+true+">"


function start() {
	simple_chart_config = {
        chart: {
            container: "#tree-simple",
            connectors: {
                type : "straight"
            },
            scrollbar: "fancy",
            animation: {
                connectorsAnimation : "linear",
                nodeAnimation : "linear",
            },
            node : {
                collapsable: false,
            }
        },
        
        nodeStructure: {
            innerHTML : "<span>" + firstNodeLabel + "</span>",
            HTMLid : firstNodeLabel,
            collapsable : false,
        }
    };

    document.body.innerHTML = "<div id='tree-simple' style='width:100%; height: 60%;'> </div>";
    my_chart = new Treant(simple_chart_config);
}

function getNodeStructure(id) {
    let spanString = "<span>" + id + "</span>"

    let jsonNODE = {
        HTMLid : id,
        innerHTML: spanString,
        collapsable:false
    }

    return jsonNODE;
}

function constructTree(graph, node) {
	start()
	let markedNodes = new Array()
	let searchOrder = new Array()

	file = new Array()
	file.push(node)

	markedNodes.push(node.id)

	while(file.length !== 0) {
		node = file.shift();
		searchOrder.push(node.id)
		node.children.forEach((child) => {
			if(!markedNodes.includes(child.id)) {
				file.push(child)
				markedNodes.push(child.id)

				let parentNode = my_chart.tree.nodeDB.db.find(obj => {
			        return obj.nodeHTMLid === node.id;
			    })
			    console.log("parentNode", parentNode)
			    let childNode = getNodeStructure(child.id)
			    console.log("childNode", childNode)
				let nextNode = my_chart.tree.addNode(parentNode, childNode);
				console.log("nextNode", nextNode);
			}
		})
	}
	console.log(simple_chart_config)
}*/


function showOrderBFS(nodeList) {
	if(document.getElementById("output-BFS") !== null) document.getElementById("output-BFS").innerHTML += (nodeList.join(' - '))
}

function showOrderDFS(nodeList) {
	document.getElementById("output-DFS").innerHTML += "<br>" + (nodeList.join(' - '))
}

function showShortestPath(string) {
	document.getElementById("ShortestPath").innerHTML += "<br>" + string
}

function BFS(graph, node, goal) {
	let markedNodes = new Array()
	let searchOrder = new Array()

	file = new Array()
	file.push(node)

	markedNodes.push(node.id)

	const edges = [];
    edges[node.id] = 0;

    const predecessors = [];
    predecessors[node.id] = null;

    const buildPath = (goal, root, predecessors) => {
	    const stack = [];
	    stack.push(goal.id);
	    let u = predecessors[goal.id];
	    while(u !== null && u.id !== root.id) {
	        stack.push(u.id);
	        u = predecessors[u.id];
	    }

	    let path = stack.reverse().join(' - ');
	    showShortestPath(path);
	}

	while(file.length !== 0) {
		node = file.shift();
		searchOrder.push(node.id)

		if (node.id === goal.id) { 
            buildPath(goal, node, predecessors)
           
        }

		node.children.forEach((child) => {
			if(!markedNodes.includes(child.id)) {
				file.push(child)
				markedNodes.push(child.id)
				edges[child.id] = edges[node] + 1;
				predecessors[child.id] = node;
			}
		})
	}
	showOrderBFS(searchOrder)
}


function explore(graph, node, markedNodes) {
	markedNodes.push(node.id)
	node.children.forEach((child) => {
		if(!markedNodes.includes(child.id)) {
			explore(graph, child, markedNodes)
		}	
	})
}

function DFS(graph, node, markedNodes) {
	document.getElementById("output-DFS").style.display = "flex";
	document.getElementById("output-DFS").innerHTML += "DFS : "
	if(!markedNodes.includes(node.id)) {
		explore(graph, node, markedNodes)
	}
	graph.nodeList.forEach(node => {
		if(!markedNodes.includes(node.id)) {
			explore(graph, node, markedNodes)
		}
	})
	showOrderDFS(markedNodes)
}


function showDFS() {
	set(3)
	var title = document.getElementById("title"); document.body.removeChild(title);
	var context = document.getElementById("context"); document.body.removeChild(context);
	var buttons = document.getElementById("buttons"); document.body.removeChild(buttons);
	var bfs = document.getElementById("output-BFS"); document.body.removeChild(bfs);
	document.getElementById("ShortestPath").style.display = "flex";

	let nodeList = new Array()
	let firstNode = new Node("<3-3-"+true+">")
	let goalNode = new Node("<0-0-"+false+">")
	let graph = buildGraph(initialState, firstNode, nodeList)

	let markedNodes = new Array()
	DFS(graph, firstNode, markedNodes)
	console.log(graph)

	BFS(graph, firstNode, goalNode)
	//constructTree(graph, firstNode)

}

function showBFS() {
	set(3)
	var title = document.getElementById("title"); document.body.removeChild(title);
	var context = document.getElementById("context"); document.body.removeChild(context);
	var buttons = document.getElementById("buttons"); document.body.removeChild(buttons);
	var dfs = document.getElementById("output-DFS"); document.body.removeChild(dfs);
	document.getElementById("output-BFS").style.display = "flex";
	document.getElementById("output-BFS").innerHTML += "BFS : "
	document.getElementById("ShortestPath").style.display = "flex";

	let nodeList = new Array()
	let firstNode = new Node("<3-3-"+true+">")
	let goalNode = new Node("<0-0-"+false+">")
	let graph = buildGraph(initialState, firstNode, nodeList)

	console.log(graph)

	let markedNodes = new Array()
	BFS(graph, firstNode, goalNode)
}



/*nodeList = [
nodeA = new Node("A"),
nodeB = new Node("B"),
nodeC = new Node("C"),
nodeD = new Node("D"),
nodeE = new Node("E"),
]

nodeA.appendChild(nodeB, nodeC)
nodeB.appendChild(nodeA, nodeD)
nodeC.appendChild(nodeA, nodeE)
nodeD.appendChild(nodeA)
nodeE.appendChild(nodeA)

graph = new Graph(nodeList)

//BFS
BFS(graph, nodeA)

//DFS
let markedNodes = new Array()
DFS(graph, nodeA, markedNodes)*/



