class Graph {

	constructor(nodeList) {
		this.nodeList = nodeList
	}
}

function showOrder(nodeList) {
	console.log(nodeList.join('-'))
}

function BDS(graph, node) {
	console.log("BDS : ")
	markedNodes = new Array()
	searchOrder = new Array()

	file = new Array()
	file.push(node)

	markedNodes.push(node.id)

	while(file.length !== 0) {
		node = file.shift();
		searchOrder.push(node.id)
		node.children.forEach((child) => {
			if(!markedNodes.includes(child)) {
				file.push(child)
				markedNodes.push(child)
			}
		})
	}
	showOrder(searchOrder)
}


function explore(graph, node, markedNodes, searchOrder) {
	searchOrder.push(node.id)
	markedNodes.push(node)
	node.children.forEach((child) => {
		if(!markedNodes.includes(child)) {
			explore(graph, child, markedNodes, searchOrder)
		}	
	})
}

function DFS(graph, node, markedNodes, searchOrder) {
	console.log("DFS : ")
	graph.nodeList.forEach(node => {
		if(!markedNodes.includes(node)) {
			explore(graph, node, markedNodes, searchOrder)
		}
	})
	showOrder(searchOrder)
}

nodeA = new Node("A")
nodeB = new Node("B")
nodeC = new Node("C")
nodeD = new Node("D")
nodeE = new Node("E")
nodeF = new Node("F")
nodeG = new Node("G")

nodeB.appendChild(nodeD, nodeF)
nodeC.appendChild(nodeG)
nodeE.appendChild(nodeF)
nodeA.appendChild(nodeB, nodeC, nodeE)

nodeList = [nodeA, nodeB, nodeC, nodeD, nodeE, nodeF, nodeG]

graph = new Graph(nodeList)

/*nodeB.drawChildren()
nodeC.drawChildren()
nodeE.drawChildren()
nodeA.drawChildren()*/

//BDS
BDS(graph, nodeA)

//DFS
markedNodes = new Array()
searchOrder = new Array()
DFS(graph, nodeA, markedNodes, searchOrder)



