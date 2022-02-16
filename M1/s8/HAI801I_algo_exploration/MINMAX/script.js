function result(state, action) {
	state[action.pos] = action.value
	return state
}

function utility(state) {
	if state.lose() return -1
	if state.win() return 1
	else return 0
}

function maxValue(state) {
	if(isTerminal(state)) return utility(state)
	let u = MIN_VALUE
	state.actions.forEach(a => {
		u = Math.max(u, minValue(result(state, a)))
	})
	return u
}

function minValue(state) {
	if(isTerminal(state)) return utility(state)
	let u = MAX_VALUE
	state.actions.forEach(a => {
		u = Math.min(u, maxValue(result(state, a)))
	})
	return u
}

function minMax(state) {
	let action = state.actions.getMax()
	return minValue(result(state, action))
}