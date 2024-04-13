#include <iostream>
#include <queue>
#include <vector>
#include <utility>

using namespace std;

int chess[41][41];
int chessKignht[41][41];
int L, N, Q;

int answer = 0;
bool flag = true;


int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

struct King {
	int i, d;
};

class Knight{
public :
	int number, r, c, h, w, k;
	int nc, nr;
	bool live;
	vector<pair<int, int> > v;
	bool isMove;

	Knight() {}
	Knight(int n, int r, int c, int h, int w, int k)
	: number(n), r(r), c(c), h(h), w(w), k(k) {
		live = true;
		isMove = false;
		setRectangle();
	}

	void setUpdate() {
		this->r = nr;
		this->c = nc;
	}

	void setNew(int nr, int nc) {
		this->nr = nr;
		this->nc = nc;
		isMove = true;
	}

	void setRectangle() {
		v.clear();
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				chessKignht[r+i][c+j] = number;
				v.push_back(make_pair(r+i, c+j));
			}
 		}
	}
};

Knight knights[31];
King king[101];

bool isValid(int r, int c) {
	if (r < 1 || L < r || c < 1 || L < c) return false;
	return true;
}

void move(int i, int d) {
	int tmp[41][41] = {0,};
	queue<int> q;
	queue<int> q2;

	// for (int r = 1; r <= L; r++) {
	// 	for (int c = 1; c <= L; c++) {
	// 		tmp[r][c] = chessKignht[r][c];
	// 	}
	// }
	q.push(i);
	// q2.push(i);

	while (q.size() > 0) {

		int number = q.front();
		q.pop();

		int nr = knights[number].r + dr[d];
		int nc = knights[number].c + dc[d];

		knights[number].setNew(nr, nc);

		for (int i = 0; i < knights[number].v.size(); i++) {
			int r = knights[number].v[i].first;
			int c = knights[number].v[i].second;
			nr = r + dr[d];
			nc = c + dc[d];

			if (!isValid(nr, nc) || chess[nr][nc] == 2) {
				flag = false;
				break;
			}

			if (chessKignht[nr][nc] > 0 && chessKignht[nr][nc] != number) { 
				q.push(chessKignht[nr][nc]);
				// q2.push(chessKignht[nr][nc]);
			}
			// tmp[r][c] = 0;
			tmp[nr][nc] = number;
			flag = true; 
			
		}
	}

	if (flag) {
		for (int j = 1; j <= L; j++) {
			for (int k = 1; k <= L; k++) {
				chessKignht[j][k] = tmp[j][k];
			}
		}

		for (int j = 1; j <= N; j++) {
			if (knights[j].live) {
				knights[j].setUpdate();
				knights[j].setRectangle();
			}
		}
	} else {
		for (int j = 1; j <= N; j++) {
			if (knights[i].isMove == true)
				knights[i].isMove = false;
		}
	}
}

void damage(int index) {
	queue<int> q;
	// cout << "index : " << index << endl;
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			if (chess[i][j] == 1 
				&& chessKignht[i][j] > 0
				&& chessKignht[i][j] != index
				&& knights[chessKignht[i][j]].isMove) {
				// cout << chessKignht[i][j] << endl;
				knights[chessKignht[i][j]].k--;
				answer++;
				if (knights[chessKignht[i][j]].k <= 0) {
					answer--;
					// cout << "die " << chessKignht[i][j] << endl;
					knights[chessKignht[i][j]].live = false;
					q.push(chessKignht[i][j]);
				}
			}
		}
	}

	while (q.size() > 0) {
		int number = q.front();
		for (int i = 0; i < knights[number].v.size(); i++) {
			chessKignht[knights[number].v[i].first][knights[number].v[i].second] = 0;
		}
		q.pop();
	}
}

int main() {
	cin >> L >> N >> Q;

	for (int i =1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> chess[i][j];
		}
	}

	for (int i =1; i < N+1; i++) {
		int r, c, h, w, k;
		cin >> r >> c>> h >> w>> k;
		Knight kn = Knight(i, r, c, h, w, k);
		knights[i] = kn;
	}

	for (int k = 1; k <= Q; k++) {
		int i, d;
		cin >> i >> d;
		King q = {i, d};
		king[i] = q;
	}

	for (int k = 1; k <= Q; k++) {
		if (!knights[king[k].i].live) continue;

		move(king[k].i, king[k].d);
		// cout << "\n====== " << k << " =========\n";

		// cout << "move\n";
		// for (int i =1; i <= L; i++) {
		// 	for (int j = 1; j <= L; j++) {
		// 		cout << chessKignht[i][j] << " ";
		// 	}cout << endl;
		// }cout << endl;
		if (flag)
			damage(king[k].i);

		// cout << "damage\n";

		for (int i = 1; i <= N; i++) {
			knights[i].isMove = false;
		}

		// for (int i =1; i <= L; i++) {
		// 	for (int j = 1; j <= L; j++) {
		// 		cout << chessKignht[i][j] << " ";
		// 	}cout << endl;
		// }

	}

	cout << answer << '\n';

	return 0;
}