var inputValue = null
var nbPeople = null
var initialState = null
var finalState = null

var outputGraph = document.getElementById("outputGraph")

function set(value) {
	inputValue = value;
	nbPeople = parseInt(inputValue);
	initialState = [nbPeople, nbPeople, true]
	finalState = [0, 0, false]
}

function printState(state, isChild) {
	return isChild ? "<span>Etat enfant : " + '\t<' + state.join('-') + '>' + "</span>" : "<span>Etat actuel : <" + state.join('-') + '>' + "</span>"
}

function buildGraph(state, node, nodeList) {
	outputGraph.innerHTML += printState(state, false)
	console.log("Etat actuel : " + '<' + state.join('-') + '>')
	if(nodeList.map(nodeFromList => nodeFromList.id).includes(node.id)) {
		outputGraph.innerHTML += "<span>Ce node est déja présent (On s'arrête ici)</span>"
		console.log("Ce node est déja présent (On s'arrête ici)")
		let sameNode = nodeList.filter(nodeFromList => nodeFromList.id === node.id)[0]
		sameNode.children.forEach(child => {
			node.appendChild(child)
		})
	}
	else{
		nodeList.push(node)
		let childState1 = []; let childState2 = []; let childState3 = []; let childState4 = []; let childState5 = []
		let label1, label2, label3, label4, label5
		//Si le bateau est sur la rive de départ
		if(state[2]) {
			//1 missionnaire en moins
			childState1[0] = state[0]-1
			childState1[1] = state[1]
			childState1[2] = !state[2]
			if(respectRule(childState1)) {
				let label1 = '<' + childState1.join('-') + '>'
				console.log("\tEtat enfant : " + label1)
				outputGraph.innerHTML += printState(childState1, true)
				let childNode1 = new Node(label1)
				node.appendChild(childNode1)
				buildGraph(childState1, childNode1, nodeList)
			}

			//2 missionnaire en moins
			childState2[0] = state[0]-2
			childState2[1] = state[1]
			childState2[2] = !state[2]
			if(respectRule(childState2)) {
				let label2 = '<' + childState2.join('-') + '>'
				console.log("\tEtat enfant : " + label2)
				outputGraph.innerHTML += printState(childState2, true)
				let childNode2 = new Node(label2)
				node.appendChild(childNode2)
				buildGraph(childState2, childNode2, nodeList)
			}

			//1 missionnaire et 1 cannibale en moins
			childState3[0] = state[0]-1
			childState3[1] = state[1]-1
			childState3[2] = !state[2]
			if(respectRule(childState3)) {
				let label3 = '<' + childState3.join('-') + '>'
				console.log("\tEtat enfant : " + label3)
				outputGraph.innerHTML += printState(childState3, true)
				let childNode3 = new Node(label3)
				node.appendChild(childNode3)
				buildGraph(childState3, childNode3, nodeList)
			}

			//1 cannibale en moins
			childState4[0] = state[0]
			childState4[1] = state[1]-1
			childState4[2] = !state[2]
			if(respectRule(childState4)) {
				let label4 = '<' + childState4.join('-') + '>'
				console.log("\tEtat enfant : " + label4)
				outputGraph.innerHTML += printState(childState4, true)
				let childNode4 = new Node(label4)
				node.appendChild(childNode4)
				buildGraph(childState4, childNode4, nodeList)
			}

			//2 cannibale en moins
			childState5[0] = state[0]
			childState5[1] = state[1]-2
			childState5[2] = !state[2]
			if(respectRule(childState5)) {
				let label5 = '<' + childState5.join('-') + '>'
				console.log("\tEtat enfant : " + label5)
				outputGraph.innerHTML += printState(childState5, true)
				let childNode5 = new Node(label5)
				node.appendChild(childNode5)
				buildGraph(childState5, childNode5, nodeList)
			}
		}
		//Si le bateau est sur la bonne rive
		if(!state[2]) {
			//1 missionnaire en plus
			childState1[0] = state[0]+1
			childState1[1] = state[1]
			childState1[2] = !state[2]
			if(respectRule(childState1)) {
				let label1 = '<' + childState1.join('-') + '>'
				console.log("\tEtat enfant : " + label1)
				outputGraph.innerHTML += printState(childState1, true)
				let childNode1 = new Node(label1)
				node.appendChild(childNode1)
				buildGraph(childState1, childNode1, nodeList)
			}

			//2 missionnaire en plus
			childState2[0] = state[0]+2
			childState2[1] = state[1]
			childState2[2] = !state[2]
			if(respectRule(childState2)) {
				let label2 = '<' + childState2.join('-') + '>'
				console.log("\tEtat enfant : " + label2)
				outputGraph.innerHTML += printState(childState2, true)
				let childNode2 = new Node(label2)
				node.appendChild(childNode2)
				buildGraph(childState2, childNode2, nodeList)
			}

			//1 missionnaire et 1 cannibale en plus
			childState3[0] = state[0]+1
			childState3[1] = state[1]+1
			childState3[2] = !state[2]
			if(respectRule(childState3)) {
				let label3 = '<' + childState3.join('-') + '>'
				console.log("\tEtat enfant : " + label3)
				outputGraph.innerHTML += printState(childState3, true)
				let childNode3 = new Node(label3)
				node.appendChild(childNode3)
				buildGraph(childState3, childNode3, nodeList)
			}

			//1 cannibale en plus
			childState4[0] = state[0]
			childState4[1] = state[1]+1
			childState4[2] = !state[2]
			if(respectRule(childState4)) {
				let label4 = '<' + childState4.join('-') + '>'
				console.log("\tEtat enfant : " + label4)
				outputGraph.innerHTML += printState(childState4, true)
				let childNode4 = new Node(label4)
				node.appendChild(childNode4)
				buildGraph(childState4, childNode4, nodeList)
			}

			//2 cannibale en plus
			childState5[0] = state[0]
			childState5[1] = state[1]+2
			childState5[2] = !state[2]
			if(respectRule(childState5)) {
				let label5 = '<' + childState5.join('-') + '>'
				console.log("\tEtat enfant : " + label5)
				outputGraph.innerHTML += printState(childState5, true)
				let childNode5 = new Node(label5)
				node.appendChild(childNode5)
				buildGraph(childState5, childNode5, nodeList)
			}
		}
		return new Graph(nodeList)
	}
}

function respectRule(state) {
	if (state[0] > nbPeople || state[0] < 0) {
		return false
	}
	if (state[1] > nbPeople || state[1] < 0) {
		return false
	}
	if(state[0] < state[1] && state[0] == 0){ return true
	}
	if (state[0] < state[1]) {
		return false
	}
	if((nbPeople - state[0] < nbPeople - state[1]) && (nbPeople - state[0] == 0)){return true
	}
	if(nbPeople - state[0] < nbPeople - state[1] && state[0] > 0) {
		return false
	}
	return true
}

/*let nodeList = []
let node = new Node("<3-3-"+true+">")
let graph = buildGraph(initialState, node, nodeList)*/
//graph.nodeList.forEach(node => node.drawChildren())
