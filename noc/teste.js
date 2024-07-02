const blockedCells = new Array(8).fill(new Array(8));
for(let i = 0; i < 8; i++){
	for(let j = 0; j < 8; j++){
		blockedCells[j][i] = { x: i, y: j };
	}
}

console.log(JSON.stringify(blockedCells.));
