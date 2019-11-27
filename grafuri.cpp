// grafuri.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

using namespace std;


int min(int a,int b)
{
	if (a < b) return a;
	else return b;
}

void s_read(istream& in, int& n)
{
	in.exceptions(std::ios_base::failbit);
	try {
		in >> n;
	}
	catch (const std::ios_base::failure & failure) {
		in.clear();
		in.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Input Gresit" << endl;
		s_read(in, n);
	}
	if (n < 0) n = 0;
}

template<class t>
void ts_read(istream& in, t& n)
{
	try {
		in >> n;
	}
	catch (const std::ios_base::failure & failure) {
		in.clear();
		in.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "input gresit" << endl;
		ts_read<t>(in, n);
	}
}




template <class t>
class Matrice {
private:
	t** a;
	int dimensiune=0;
public:
	Matrice()
	{
		a = new t*[1];
		a[0] = new t[1];
	}
	
	Matrice(int d)
	{
		if (d < 0) d = 0;
		dimensiune = d;
		a = new t*[d];

		for (int i = 0; i < d; i++)
		{
			a[i] = new t[d];
		}
		zeros();
	}

	Matrice(Matrice<t> &aux)
	{
		dimensiune = aux.dimensiune;
		a = new t* [aux.dimensiune];
		int d = aux.dimensiune;
		for (int i = 0; i < d; i++)
		{
			a[i] = new t[d];
		}

		for (int i = 0; i < d; i++)
			for (int j = 0; j < d; j++)
				a[i][j] = aux.a[i][j];
	}






	~Matrice(){
		for (int i = 0; i < dimensiune; i++)
			delete a[i];

		delete a;
	}

	t* operator [](int poz)
	{
		return a[poz];
	}

	friend istream &operator >>(istream &in,Matrice &m)
	{
		int n;
		cout << "dimensiune:";
		
		s_read(in, n);

		m = m + (n - m.dimensiune);
		for (int i = 0; i < m.dimensiune; i++)
		{
			for (int j = 0; j < m.dimensiune; j++)
			{
				ts_read<t>(in, m.a[i][j]);
			}
		}
		return in;
	}

	friend ostream& operator <<(ostream &out,Matrice &m)
	{
		for (int i = 0; i < m.dimensiune; i++)
		{
			for (int j = 0; j < m.dimensiune; j++)
			{
				out<< m.a[i][j]<<" ";
			}
			out << endl;
		}
		return out;
	}

	Matrice<t> &operator+(int a)
	{
		
		Matrice aux(dimensiune+a);
		for (int i = 0; i < aux.dimensiune; i++)
			for (int j = 0; j < aux.dimensiune; j++)
				aux[i][j] = 0;
		for (int i = 0; i <min(a, dimensiune); i++)
			for (int j = 0; j <min(a, dimensiune); j++)
				aux.a[i][j] = this->a[i][j];
		return *new Matrice(aux);
	}

	Matrice<t>& operator=(Matrice<t> &aux)
	{
		dimensiune = aux.dimensiune;
		a = new t * [aux.dimensiune];
		int d = aux.dimensiune;
		for (int i = 0; i < d; i++)
		{
			a[i] = new t[d];
		}

		for (int i = 0; i < d; i++)
			for (int j = 0; j < d; j++)
				a[i][j] = aux.a[i][j];
		return *this;
	}

	int get_dim(){
		return dimensiune;
	}

	void zeros()
	{
		for (int i = 0; i < dimensiune; i++)
			for (int j = 0; j < dimensiune; j++)
				a[i][j] = 0;
	}

	void ones() {
		for (int i = 0; i < dimensiune; i++)
			for (int j = 0; j < dimensiune; j++)
				a[i][j] = 1;
	}


	Matrice<t> &trans()
	{
		Matrice<t> aux = *this;
		for (int i = 0; i < dimensiune; i++)
			for (int j = 0; j < dimensiune; j++)
				aux.a[i][j] = a[j][i];
		return *new Matrice<t>(aux);
	}

};



class Graf {
protected:
	Matrice<bool> adiacenta;
	int nr_elemente;
	string name;
public:


	void s_name(string s)
	{
		name.assign(s);
	}

	string g_name()
	{
		return name;
	}


	Graf()
	{
		nr_elemente = 0;
	}
	Graf(int nr_el)
	{
		
		nr_elemente = nr_el;
		adiacenta = *new Matrice<bool>(nr_elemente);
	}

	Graf& operator =(Graf& g)
	{
		this->adiacenta = g.adiacenta;
		this->nr_elemente = g.nr_elemente;
		return *this;

	}


	virtual void afisare_muchii() = 0;

};

class Graf_complet:virtual protected Graf
{

protected:
	int nr_muchii;
public:
	

	Graf_complet() :Graf()
	{
		name.assign(typeid(*this).name());
		nr_muchii = 0;
	}

	Graf_complet(int dim):Graf(dim)
	{
		name.assign(typeid(*this).name());
		if (dim < 0) dim = 0;
		nr_muchii = nr_elemente * (nr_elemente - 1) / 2;
	}

	Graf_complet(Graf_complet& g) :Graf(g)
	{
		nr_muchii = g.nr_muchii;
		name.assign(typeid(*this).name());
	}

	friend istream& operator>>(istream &in,Graf_complet&g)
	{
		s_read(in,g.nr_elemente);
		g.reprocesare();
		return in;
	}

	friend ostream& operator<<(ostream& ost, Graf_complet& g)
	{
		ost << "nr elem: " << g.nr_elemente << " nr muchii: " << g.nr_muchii<<endl;
		return ost;
	}

	Graf_complet& operator =(Graf_complet& g)
	{
		this->adiacenta = g.adiacenta;
		this->nr_elemente = g.nr_elemente;
		return *this;

	}

	void reprocesare()
	{
		nr_muchii = nr_elemente * (nr_elemente - 1) / 2;
	}


	virtual void afisare_muchii()
	{
		cout << nr_muchii;
	}

};

class Graf_antisimetric : virtual protected Graf
{
public:
	Graf_antisimetric() :Graf()
	{
		name.assign(typeid(*this).name());
	}

	Graf_antisimetric(Graf_antisimetric& g) :Graf(g)
	{
		name.assign(typeid(*this).name());
	}

	
	
	Graf_antisimetric(Matrice<bool> conexiuni,bool complet=false)
	{
		name.assign(typeid(*this).name());
		adiacenta = conexiuni;
		nr_elemente = conexiuni.get_dim();
		if (complet) adiacenta.ones();
	}


	friend istream& operator>>(istream& in, Graf_antisimetric& g)
	{
		Matrice<bool> mat;
		in >> mat;
		g = *new Graf_antisimetric(mat);
		return in;
	}

	friend ostream& operator<<(ostream& ost, Graf_antisimetric& g)
	{
		ost << "{";
		for (int i = 0; i < g.nr_elemente; i++)
			for (int j = 0; j < g.nr_elemente; j++)
			{
				if (g.adiacenta[i][j]) ost << "[ " << i << " conectat cu " << j << "] ";
			}
		ost << "}";
		return ost;
	}


	Graf_antisimetric &operator =(Graf_antisimetric &g)
	{
		this->adiacenta = g.adiacenta;
		this->nr_elemente = g.nr_elemente;
		return *this;
		
	}	

	virtual void afisare_muchii()
	{
		cout <<"{" ;
		for (int i = 0; i < nr_elemente; i++)
			for (int j = 0; j < nr_elemente; j++)
			{
				if (adiacenta[i][j]) cout << "[ " << i << "->" << j << "] ";
			}
		cout << "}";
	}
};

class Graf_turneu :protected Graf_complet, protected Graf_antisimetric
{

public:

	Graf_turneu() :Graf_antisimetric(), Graf_complet(),Graf(){
		name.assign(typeid(*this).name());
		nr_muchii = nr_elemente * nr_elemente;
	}

	Graf_turneu(Matrice<bool>& m) :Graf_antisimetric(m,true), Graf_complet(m.get_dim()),Graf(m.get_dim()) {
		name.assign(typeid(*this).name());
		nr_muchii = nr_elemente * nr_elemente;
	}
	Graf_turneu(int s) :Graf_antisimetric(*new Matrice<bool>(s),true),Graf_complet(s),Graf(s)
	{
		name.assign(typeid(*this).name());
		nr_muchii = nr_elemente * nr_elemente;
	}

	Graf_turneu(Graf_turneu& t) :Graf_antisimetric(t), Graf_complet(t),Graf(t)
	{
		name.assign(typeid(*this).name());
		nr_muchii = nr_elemente * nr_elemente;
	}

	
	friend istream& operator>>(istream& in, Graf_turneu& g)
	{
		int nr;
		in.exceptions(std::ios_base::failbit);
		s_read(in, nr);
		g = *new Graf_turneu(nr);
		return in;
	}

	friend ostream& operator<<(ostream& ost, Graf_turneu& g)
	{
		ost << "{";
		for (int i = 0; i < g.nr_elemente; i++)
			for (int j = 0; j < g.nr_elemente; j++)
			{
				if (g.adiacenta[i][j]) ost << "[ " << i << " conectat cu " << j << "] ";
			}
		ost << "}";
		ost << endl<<g.nr_muchii<<endl;
		return ost;
	}

	Graf_turneu& operator =(Graf_turneu& g)
	{
		this->adiacenta = g.adiacenta;
		this->nr_elemente = g.nr_elemente;
		return *this;

	}
	
	virtual void afisare_muchii()
	{
		
		nr_muchii = 0;
			cout << "{";
			for (int i = 0; i < nr_elemente; i++)
			{
				for (int j = 0; j < nr_elemente; j++)
				{

					if (adiacenta[i][j]) cout << "[ " << i << " ->" << j << "] ";
					nr_muchii++;
				}
				cout << endl;
			}
			cout << "}";
		
			cout <<endl<< nr_muchii << " "<<endl;
	}
	



};




void AfisareSiCitire()
{
	int n;
	cout << "nr elemente:  ";
	
	
	int i = 0;
	s_read(cin, n);
	if (n == 0) return;
	Graf** p = new Graf* [n];
	for (int i = 0; i < n; i++)
	{
		system("cls");
		cout << "optiune:\n";
		int tip = 0; 
		cout << "0-antisimetric\n1- complet\n>1: turneu\n";
		s_read(cin, tip);
		
		if (tip == 0)
		{
			Graf_antisimetric* m=new Graf_antisimetric();
			cin >> *m;
			p[i] = (Graf*) m;
		}
		else if (tip == 1)
		{
			Graf_complet* m=new Graf_complet();
			cin >> *m;
			p[i] = (Graf*)m;
		}
		else
		{
			Graf_turneu* m=new Graf_turneu();
			cin >> *m;
			p[i] = (Graf*)m;
		}
	}

	system("cls");
	for (int i = 0; i < n; i++)
	{
		cout << i << " : " << p[i]->g_name()<<endl;
		p[i]->afisare_muchii();
		cout << endl << endl;
	}

}





int main()
{
	AfisareSiCitire();
}


