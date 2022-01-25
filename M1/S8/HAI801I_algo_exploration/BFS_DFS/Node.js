class Node {

	constructor(id) {
		this.id = id
		this.children = new Array()
	}

	getId() {
		return this.id
	}

	appendChild(...nodes) {
		nodes.forEach((node) => {
			this.children.push(node)
		})
	}

	drawChildren() {
		let listChildren = new Array()
		this.children.forEach((node) => {
			listChildren.push(node.id)
		})
		let childrenPrint = listChildren.join('-')
		console.log(this.id + " : " + childrenPrint)
	}

	draw() {
		console.log(this)
	}

}