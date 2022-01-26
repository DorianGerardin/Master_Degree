class Graph {

	constructor(nodeList) {
		this.nodeList = nodeList
	}
}

function showOrderBFS(nodeList) {
	document.getElementById("output-BFS").innerHTML += (nodeList.join('-'))
}

function showOrderDFS(nodeList) {
	document.getElementById("output-DFS").innerHTML += (nodeList.join('-'))
}

function BFS(graph, node) {
	document.getElementById("output-BFS").innerHTML += "BFS : "
	markedNodes = new Array()
	searchOrder = new Array()

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


function explore(graph, node, markedNodes, searchOrder) {
	searchOrder.push(node.id)
	markedNodes.push(node.id)
	node.children.forEach((child) => {
		if(!markedNodes.includes(child.id)) {
			explore(graph, child, markedNodes, searchOrder)
		}	
	})
}

function DFS(graph, node, markedNodes, searchOrder) {
	document.getElementById("output-DFS").innerHTML += "DFS : "
	graph.nodeList.forEach(node => {
		if(!markedNodes.includes(node.id)) {
			explore(graph, node, markedNodes, searchOrder)
		}
	})
	showOrderDFS(searchOrder)
}

nodeList = [
nodeA = new Node("A"),
nodeB = new Node("B"),
nodeC = new Node("C"),
nodeD = new Node("D"),
]

nodeA.appendChild(nodeB, nodeC)
nodeC.appendChild(nodeD)
nodeD.appendChild(nodeC)

graph = new Graph(nodeList)

/*nodeB.drawChildren()
nodeC.drawChildren()
nodeE.drawChildren()
nodeA.drawChildren()*/

//BFS
BFS(graph, nodeA)

//DFS
markedNodes = new Array()
searchOrder = new Array()
DFS(graph, nodeA, markedNodes, searchOrder)



