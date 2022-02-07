var inputValue = null
console.log("inputValue", inputValue)
var nbPeople = null
console.log("nbPeople", nbPeople)
var initialState = null
var finalState = null

function set(value) {
	inputValue = value;
	console.log("inputValue", inputValue)
	nbPeople = parseInt(inputValue);
	console.log("nbPeople", nbPeople)
	initialState = [nbPeople, nbPeople, true]
	finalState = [0, 0, false]
}

function buildGraph(state, node, nodeList) {
	console.log("state actuel : " + state)
	if(nodeList.map(nodeFromList => nodeFromList.id).includes(node.id)) {
		console.log("je suis un node deja present")
		let sameNode = nodeList.filter(nodeFromList => nodeFromList.id === node.id)[0]
		sameNode.children.forEach(child => {
			node.appendChild(child)
		})
	}
	else{
		nodeList.push(node)
		console.log("je push le node")
		let childState1 = []; let childState2 = []; let childState3 = []; let childState4 = []; let childState5 = []
		let label1, label2, label3, label4, label5
		//Si le bateau est sur la rive de départ
		if(state[2]) {
			//1 missionnaire en moins
			childState1[0] = state[0]-1
			childState1[1] = state[1]
			childState1[2] = !state[2]
			if(respectRule(childState1)) {
				console.log("\tstate enfant1 : " + childState1)
				let label1 = '<' + childState1.join('-') + '>'
				let childNode1 = new Node(label1)
				node.appendChild(childNode1)
				buildGraph(childState1, childNode1, nodeList)
			}

			//2 missionnaire en moins
			childState2[0] = state[0]-2
			childState2[1] = state[1]
			childState2[2] = !state[2]
			if(respectRule(childState2)) {
				console.log("\tstate enfant2 : " + childState2)
				let label2 = '<' + childState2.join('-') + '>'
				let childNode2 = new Node(label2)
				node.appendChild(childNode2)
				buildGraph(childState2, childNode2, nodeList)
			}

			//1 missionnaire et 1 cannibale en moins
			childState3[0] = state[0]-1
			childState3[1] = state[1]-1
			childState3[2] = !state[2]
			if(respectRule(childState3)) {
				console.log("\tstate enfant3 : " + childState3)
				let label3 = '<' + childState3.join('-') + '>'
				let childNode3 = new Node(label3)
				node.appendChild(childNode3)
				buildGraph(childState3, childNode3, nodeList)
			}

			//1 cannibale en moins
			childState4[0] = state[0]
			childState4[1] = state[1]-1
			childState4[2] = !state[2]
			if(respectRule(childState4)) {
				console.log("\tstate enfant4 : " + childState4)
				let label4 = '<' + childState4.join('-') + '>'
				let childNode4 = new Node(label4)
				node.appendChild(childNode4)
				buildGraph(childState4, childNode4, nodeList)
			}

			//2 cannibale en moins
			childState5[0] = state[0]
			childState5[1] = state[1]-2
			childState5[2] = !state[2]
			if(respectRule(childState5)) {
				console.log("\tstate enfant5 : " + childState5)
				let label5 = '<' + childState5.join('-') + '>'
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
				console.log("je rajoute un missionnaire")
				console.log("\tstate enfant1 : " + childState1)
				let label1 = '<' + childState1.join('-') + '>'
				let childNode1 = new Node(label1)
				node.appendChild(childNode1)
				buildGraph(childState1, childNode1, nodeList)
			}

			//2 missionnaire en plus
			childState2[0] = state[0]+2
			childState2[1] = state[1]
			childState2[2] = !state[2]
			if(respectRule(childState2)) {
				console.log("\tstate enfant2 : " + childState2)
				let label2 = '<' + childState2.join('-') + '>'
				let childNode2 = new Node(label2)
				node.appendChild(childNode2)
				buildGraph(childState2, childNode2, nodeList)
			}

			//1 missionnaire et 1 cannibale en plus
			childState3[0] = state[0]+1
			childState3[1] = state[1]+1
			childState3[2] = !state[2]
			if(respectRule(childState3)) {
				console.log("\tstate enfant3 : " + childState3)
				let label3 = '<' + childState3.join('-') + '>'
				let childNode3 = new Node(label3)
				node.appendChild(childNode3)
				buildGraph(childState3, childNode3, nodeList)
			}

			//1 cannibale en plus
			childState4[0] = state[0]
			childState4[1] = state[1]+1
			childState4[2] = !state[2]
			if(respectRule(childState4)) {
				console.log("\tstate enfant4 : " + childState4)
				let label4 = '<' + childState4.join('-') + '>'
				let childNode4 = new Node(label4)
				node.appendChild(childNode4)
				buildGraph(childState4, childNode4, nodeList)
			}

			//2 cannibale en plus
			childState5[0] = state[0]
			childState5[1] = state[1]+2
			childState5[2] = !state[2]
			if(respectRule(childState5)) {
				console.log("\tstate enfant5 : " + childState5)
				let label5 = '<' + childState5.join('-') + '>'
				let childNode5 = new Node(label5)
				node.appendChild(childNode5)
				buildGraph(childState5, childNode5, nodeList)
			}
		}
		return new Graph(nodeList)
	}
}

function respectRule(state) {
	console.log(state + " respecte ? : ")
	if (state[0] > nbPeople || state[0] < 0) {
		console.log("false"); return false
	}
	if (state[1] > nbPeople || state[1] < 0) {
		console.log("false"); return false
	}
	if(state[0] < state[1] && state[0] == 0){ console.log("true"); return true
	}
	if (state[0] < state[1]) {
		console.log("false"); return false
	}
	if((nbPeople - state[0] < nbPeople - state[1]) && (nbPeople - state[0] == 0)){ console.log("true"); return true
	}
	if(nbPeople - state[0] < nbPeople - state[1] && state[0] > 0) {
		console.log("false"); return false
	}
	console.log("true")
	return true
}

/*let nodeList = []
let node = new Node("<3-3-"+true+">")
let graph = buildGraph(initialState, node, nodeList)*/
//graph.nodeList.forEach(node => node.drawChildren())
