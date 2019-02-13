
struct Node {
	char letter;
	int row;
	int column;
	int color;
	bool visited;

	Node() {
		letter = '\0';
		row = 0;
		column = 0;
		visited = false;
		color = 0;
	};
};