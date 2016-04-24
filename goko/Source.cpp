#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define ARRAY_NUM 1002
#define BASE 10

void sum(int[], int[]);
void sub(int[], int[]);
void div(int[], int[], int[]);
void mul(int[], int[]);
int find_max(int[]);
int find_maxmulti(int[], int[]);
int compare(int[], int[]);
void shift(int[], int, int[]);
void shiftr(int[], int, int[]);
void init(int[]);
void printer(int[]);
void write(int[]);
void format(int[], int);
void arctan(int, int, int[]);


// g
void sum(int a[], int b[]){

	int carry = 0;
	for (int i = 0; i < ARRAY_NUM; i++){
		a[i] += (b[i] + carry);
		if (a[i] >= BASE){
			a[i] -= BASE;
			carry = 1;
		}
		else{
			carry = 0;
		}
	}

}

// g
void sub(int a[], int b[]){

	int carry = 0;
	for (int i = 0; i < ARRAY_NUM; i++){
		a[i] -= (b[i] + carry);
		if (a[i] < 0){
			a[i] += BASE;
			carry = 1;
		}
		else {
			carry = 0;
		}
	}

}

// g
void mul(int a[], int b[]){

	int a_buf[ARRAY_NUM];
	memcpy(a_buf, a, sizeof(int)*ARRAY_NUM);

	int b_buf[ARRAY_NUM];
	memcpy(b_buf, b, sizeof(int)*ARRAY_NUM);

	int zero[ARRAY_NUM];
	init(zero);

	int one[ARRAY_NUM];
	init(one);
	one[0] = 1;

	int two[ARRAY_NUM];
	init(two);
	two[0] = 2;

	// bが0なら0にする
	if (compare(b, zero) == 0){
		sub(a, a_buf);
	}

	while (1){
		if (compare(b_buf, two) >= 0){
			sub(b_buf, one);
			sum(a, a_buf);
		}
		else{
			return;
		}
	}

}

void div(int a[], int b[], int q[]){

	int a_max = find_max(a);
	int b_max = find_max(b);
	int df_base = a_max - b_max;
	int buf_base = 0;
	int buf_shift[ARRAY_NUM];

	int one[ARRAY_NUM];
	init(one);
	one[0] = 1;

	int zero[ARRAY_NUM];
	init(zero);

	init(q);

	// a=0, a<b なら割らない
	if (compare(a, zero) == 0 || compare(a, b) < 0){
		q = zero;
		return;
	}

	// 被除数 > 除数 * BASE が成立する最大のBASEを探す
	for (int i = 0; i <= df_base; i++){
		shift(b, i, buf_shift);
		if (compare(a, buf_shift) >= 0){
			buf_base = i;
		}
	}

	while (1){

		// 終了チェック
		/*
		printf("start:\n");
		printer(a);
		printer(b);
		printer(q);
		printf("\n");
		*/
		if (compare(a, b) < 0){
			return;
		}

		// 被除数 > 除数 * BASE * multi が成立する最大のmultiを探す
		shift(b, buf_base, buf_shift);
		int maxmulti = find_maxmulti(a, buf_shift);
		int multi[ARRAY_NUM];
		init(multi);
		format(multi, maxmulti);

		// 被除数 - 除数 * BASE * multi を実行する
		int gsub[ARRAY_NUM];
		memcpy(gsub, buf_shift, sizeof(int)*ARRAY_NUM);
		mul(gsub, multi);
		sub(a, gsub);

		// 商を multi * BASE で求める
		int q_buf[ARRAY_NUM];
		init(q_buf);
		shift(multi, buf_base, q_buf);
		sum(q, q_buf);

		// 一周したら除数 * BASE を一桁下とす
		if (buf_base > 0){
			int b_buf[ARRAY_NUM];
			memcpy(b_buf, b, sizeof(int)*ARRAY_NUM);
			buf_base--;
		}

		/*
		printf("end:\n");
		printer(a);
		printer(b);
		printer(q);
		printf("\n");
		*/
	}

}

// g
int find_max(int a[]){

	for (int i = ARRAY_NUM - 1; i >= 0; i--){
		if (a[i] != 0){
			return i;
		}
	}
	return -1;

}


// g
int find_maxmulti(int a[], int b[]){

	int zero[ARRAY_NUM];
	init(zero);

	if (compare(a, zero) == 0 && compare(b, zero) == 0){
		return 0;
	}

	int i = 0;
	int b_buf[ARRAY_NUM];
	memcpy(b_buf, b, sizeof(int)*ARRAY_NUM);
	while (1){
		if (compare(a, b_buf) >= 0) {
			i++;
			sum(b_buf, b);
		}
		else{
			return i;
		}
	}

}

// g
int compare(int a[], int b[]){

	int a_max = find_max(a);
	int b_max = find_max(b);

	if (a_max > b_max){
		return 1;
	}
	else if (a_max < b_max){
		return -1;
	}

	for (int i = ARRAY_NUM - 1; i >= 0; i--){
		if (a[i] - b[i] != 0){
			if (a[i] - b[i] > 0){
				return 1;
			}
			else if (a[i] - b[i] < 0){
				return -1;
			}
		}
	}
	return 0;

}

// g
void shift(int a[], int n, int res[]){

	for (int i = n; i < ARRAY_NUM; i++){
		res[i] = a[i-n];
	}
	for (int i = 0; i < n; i++){
		res[i] = 0;
	}

}

void shiftr(int a[], int n, int res[]){

	init(res);
	for (int i = n; i < ARRAY_NUM - n; i++){
		res[i] = a[i+n];
	}
	for (int i = ARRAY_NUM-n; i < ARRAY_NUM; i++){
		res[i] = 0;
	}

}

// g
void init(int a[]){

	for (int i = 0; i < ARRAY_NUM; i++){
		a[i] = 0;
	}

}

// g
void printer(int a[]){

	for (int i = ARRAY_NUM - 1; i >= 0; i--){
		printf("%d", a[i]);
	}
	printf("\n");

}

void write(int a[]){

	FILE* fp;

	if ((fp = fopen("output.txt", "w")) == NULL) {
		printf("file pointer is null.");
		return;
	}

	for (int i = ARRAY_NUM - 1; i >= 0; i--){
		fprintf(fp, "%d", a[i]);
	}

}

// g
void format(int a[], int n){

	// 整数nを多倍長整数aに変換
	init(a);
	int num = 0;
	while (1){
		int q = n / 10;
		int r = n % 10;
		a[num] = r;
		num++;
		n = q;
		if (!n){
			break;
		}
	}

}

void arctan(int outer, int inner, int dst[]){

	// outer は arctan()の係数
	// inner は arctan()の引数
	// dst は計算結果

	int t[ARRAY_NUM];
	int t_buf[ARRAY_NUM];
	int s[ARRAY_NUM];
	int s_buf[ARRAY_NUM];
	int param[ARRAY_NUM];
	int parampow[ARRAY_NUM];
	int q[ARRAY_NUM];
	int i_buf[ARRAY_NUM];
	int e = 1000; // 保証桁

	init(t);
	init(t_buf);
	init(s);
	init(s_buf);
	init(param);
	init(parampow);
	init(q);
	init(i_buf);

	// 初期値の設定
	format(t_buf, outer);
	shift(t_buf, e, t);
	format(param, inner);
	format(parampow, inner); mul(parampow, param);

	// Tn, Snを求める
	div(t, param, q);	// t1(この時点ではqに入っている)
	sum(s, q);			// s1
	memcpy(t, q, sizeof(int)*ARRAY_NUM);	// tnの値をqからtに整理

	int n = 5000;		// マチンの公式の精度	
	int flag = 0;
	for (int i = 3; i < n; i += 2){
		div(t, parampow, q);					// tn
		memcpy(t, q, sizeof(int)*ARRAY_NUM);	// tnの値をqからtに整理
		format(i_buf, i);
		div(q, i_buf, s_buf);					// sの第二項の絶対値
		if (!flag){								// sの計算
			sub(s, s_buf);
			flag = 1;
		}
		else{
			sum(s, s_buf);
			flag = 0;
		}
	}

	
	memcpy(dst, s, sizeof(int)*ARRAY_NUM);

}

int main(void){

	int a[ARRAY_NUM];
	int b[ARRAY_NUM];

	arctan(16, 5, a);
	arctan(4, 239, b);
	sub(a, b);
	printer(a);
	write(a);

	return 0;

}