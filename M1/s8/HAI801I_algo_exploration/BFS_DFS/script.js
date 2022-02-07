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
	document.getElementById("output-BFS").innerHTML += (nodeList.join(' - '))
}

function showOrderDFS(nodeList) {
	document.getElementById("output-DFS").innerHTML += (nodeList.join(' - '))
}

function BFS(graph, node) {
	document.getElementById("output-BFS").innerHTML += "BFS : "
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
	set(document.getElementById("input").value)
	var title = document.getElementById("title"); document.body.removeChild(title);
	var context = document.getElementById("context"); document.body.removeChild(context);
	var form = document.getElementById("form"); document.body.removeChild(form);
	var buttons = document.getElementById("buttons"); document.body.removeChild(buttons);
	var bfs = document.getElementById("output-BFS"); document.body.removeChild(bfs);

	let nodeList = new Array()
	let firstNode = new Node("<"+inputValue+"-"+inputValue+"-"+true+">")
	let graph = buildGraph(initialState, firstNode, nodeList)

	let markedNodes = new Array()
	DFS(graph, firstNode, markedNodes)
	showOrderDFS(markedNodes)
	//constructTree(graph, firstNode)

}

function showBFS() {
	set(document.getElementById("input").value)
	var title = document.getElementById("title"); document.body.removeChild(title);
	var context = document.getElementById("context"); document.body.removeChild(context);
	var form = document.getElementById("form"); document.body.removeChild(form);
	var buttons = document.getElementById("buttons"); document.body.removeChild(buttons);
	var dfs = document.getElementById("output-DFS"); document.body.removeChild(dfs);

	let nodeList = new Array()
	let firstNode = new Node("<"+inputValue+"-"+inputValue+"-"+true+">")
	let graph = buildGraph(initialState, firstNode, nodeList)

	let markedNodes = new Array()
	BFS(graph, firstNode)
	showOrderBFS(markedNodes)
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



