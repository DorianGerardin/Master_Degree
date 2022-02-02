const initialState = [3, 3, true]
const finalState = [0, 0, false]

function buildGraph(state) {
	let label = '<' + state.join('-') + '>'
	//1 missionnaire en moins
	let childState1[3]
	childState1[0] = state[0]-1
	childState1[1] = state[1]

	 
}

function respectRule(state) {
	return state[0] <= state[1]
}