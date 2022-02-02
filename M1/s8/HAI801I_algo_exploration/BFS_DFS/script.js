function showOrderBFS(nodeList) {
	document.getElementById("output-BFS").innerHTML += (nodeList.join('-'))
}

function showOrderDFS(nodeList) {
	document.getElementById("output-DFS").innerHTML += (nodeList.join('-'))
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




nodeList = [
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

/*nodeB.drawChildren()
nodeC.drawChildren()
nodeE.drawChildren()
nodeA.drawChildren()*/

//BFS
BFS(graph, nodeA)

//DFS
let markedNodes = new Array()
DFS(graph, nodeA, markedNodes)



