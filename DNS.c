#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_CMD_LEN 5000

typedef int (*TFHash) (char*, int);

typedef struct celula2 {
	char *name;
	char *ip;          
	struct celula2 *pre, *urm;
} TCelula2, *TLDI;

typedef struct {
	int M;
	TFHash fh;
	TLDI *v;
} TH;

// Functie pentru crearea tabelei hash
TH *creare_th (int M, TFHash fh) {
	TH *th = (TH *) malloc (sizeof(TH));
	if (!th)
		return NULL;

	th->v = (TLDI *) calloc (M, sizeof(TLDI));
		
	if(!th->v) {
		free(th);
		return NULL;
	}

	th->M = M;
	th->fh = fh;
		
	return th;
}

// Functie care calculeaza functia hash
int fhash (char *key, int M) {
	int sum = 0, rest = 0;
	int init_size = strlen (key);
	int i;

	for (i = 0; i < init_size; i++)
		sum += key[i] - '\0';

	rest = sum%M;
	return rest;
}

// Functie care printeaza IP-urile elementelor din lista
void printList (TCelula2 *inregistrare, FILE *outfile) {

	for (; inregistrare != NULL; inregistrare = inregistrare->urm)
		fprintf(outfile , "%s ", inregistrare->ip); 

}

// Functie pentru printarea tabelei hash pentru fiecare lista nevida
void printHT (TH *th, FILE *outfile) {
	TLDI aux;
	int i;
	
	for (i = 0; i < th->M; i++) {
		aux = th->v[i];
		
		if (aux) {
			fprintf (outfile, "%d: ", i);
			printList (th->v[i], outfile);
			fprintf (outfile, "\n");
		}
	}
}

// Functie pentru printarea listei cu indicele index,dat ca parametru, din 
// tabela hash. 
// Daca lista asociata este vida se va afisa VIDA.
// Daca indicele index nu exista in tabela hash, nu se va afisa nimic.
void print_bucket (TH *th, char *index, FILE *outfile) {
	int i = atoi (index);

	if (i >= th->M)
		return;

	if (th->v[i] != NULL) {
		printList (th->v[i], outfile);
		fprintf (outfile, "\n");

	} else 
		fprintf (outfile, "VIDA\n");

}

// Functia pentru adaugarea unui element in lista
void put (TH *th, char *key, char *value, int M) {
	int fh;
	TCelula2 *inregistrare = (TCelula2 *) malloc (sizeof(TCelula2));
	TLDI aux;

	if (!inregistrare)
		return;
	
	inregistrare->name = key;
	inregistrare->ip = value;
  inregistrare->urm = NULL;
	
	fh = th->fh(key, M);

	if (th->v[fh]) {
		// parcurg lista si verific daca elementul mai exista deja in lista, 
		// iar in acest caz, elementul nu mai trebuie introdus
		for (aux = th->v[fh]; aux != NULL; aux = aux->urm) {
			if (strcmp (aux->name, key) == 0) {
				free(inregistrare->ip);
				free(inregistrare->name);
				free(inregistrare);
				return;
			}
		}
		// daca elementul nu exista atunci parcurg lista si inserez key si value
		// ordonat dupa key
		aux = th->v[fh];
		
		if (strcmp (aux->name, key) < 0) {
			for (aux = th->v[fh]; aux->urm != NULL; aux= aux->urm) {
				if (strcmp (aux->name, key) > 0) {
					aux->pre->urm = inregistrare;
					inregistrare->urm = aux;
					inregistrare->pre = aux->pre;
					aux->pre = inregistrare;
					return;
				}
			}

			if (aux->urm == NULL) {

				if (strcmp (aux->name, key) < 0) {
					aux->urm = inregistrare;
					inregistrare->pre =aux;
					inregistrare->urm = NULL;
					return;

				} else {
					aux->pre->urm = inregistrare;
					inregistrare->urm = aux;
					inregistrare->pre = aux->pre;
					aux->pre = inregistrare;
					return;
				}
			}

		} else {
			// daca lista unde trebuie sa introduc elementul este vida, atunci
			// il leg de inceputul listei
			th->v[fh] = inregistrare;
			inregistrare->pre = th->v[fh];
			inregistrare->urm = aux;
			aux->pre = inregistrare;

			return;
		}	

	} else {
		th->v[fh] = inregistrare;
		inregistrare->pre = th->v[fh];
		return;
	}
}

// Functie ce intoarce valoarea corespunzatoare cheii "key".
// Daca "key" nu exista, intoarce NULL.
char *get (TH *th, char *key, int M) {
	int fh;
	TLDI aux;
	fh = th->fh(key, M);

	for (aux = th->v[fh]; aux != NULL; aux = aux->urm)
		if (strcmp (aux->name, key) == 0)
			return aux->ip;

	return "NULL";
}

// Functie ce cauta key in tabela hash si intoarce 1 daca exista si 0 altfel
int find (TH *th, char *key, int M) {
	int fh;
	TLDI aux;

	fh = th->fh(key, M);

	for (aux = th->v[fh]; aux != NULL; aux = aux->urm)
		if (strcmp (aux->name, key) == 0)
			return 1;

	return 0;
}

// Functie ce sterge elementul din tabela hash.
// Daca elementul ce se doreste a fi sters nu exista, atunci nu se mai
// face stergerea.
void rem (TH *th, char *key, int M) {
	int fh;
	TLDI aux, aux1;

	fh = th->fh(key, M);

	// Nu stim daca v[fh] exista (ar putea sa fie NULL)
	// daca e NULL, elementul nu exista in hash table si se returneaza functia
	if (th->v[fh] == NULL)
		return;

	if (strcmp (th->v[fh]->name, key) == 0) {
		aux = th->v[fh];
		th->v[fh] = th->v[fh]->urm;

		free (aux->name);
		free (aux->ip);
		free (aux);

		return;
	}
	
	for (aux = th->v[fh]; aux->urm != NULL; aux = aux->urm) {
		aux1 = aux->urm;

		if (strcmp (aux1->name, key) == 0) {
			aux->urm = aux1->urm;
			free (aux1->name);
			free (aux1->ip);
			free (aux1);
			break;
		}
	}
}

int main (int argc, char *argv[]) {
	char *token, *key, *value;
	char script_line[MAX_CMD_LEN];
	char delimiters[] = " \n";
	
	FILE *infile = fopen (argv[2], "r");
	FILE *outfile = fopen (argv[3], "w");
	
	int M = atoi (argv[1]);
	int i;
	TH *th = NULL;
	TLDI aux, k;
	th = creare_th (M, fhash);
	if(th == NULL)
		return 0;
	
	// citeste linie cu linie din fisierul de input si scrie in fisierul de 
	// output. In functie de comanda de pe fiecare linie, intra intr-un if si
	// apeleaza functiile necesare
	while (fgets (script_line, MAX_CMD_LEN, infile)) {
		token = strtok (script_line, delimiters);
    
		// token o sa fie NULL, ceea ce cauzeaza segfault daca apelam strcmp pe el.
		// Daca token e NULL, iesim din bucla
		if (token == NULL)
			break;

		if (strcmp (token, "put") == 0) {
			token = strtok (NULL, delimiters);
			key= strdup (token);

			token = strtok (NULL, delimiters);
			value = strdup (token);

			put(th, key, value, M);

		} else if (strcmp (token, "get") == 0) {
			token = strtok (NULL, delimiters);
			key= strdup (token);

			char *ok = get (th, key, M);
			
			fprintf (outfile, "%s\n", ok);

			free ( key);

		} else if (strcmp (token, "remove") == 0) {
			token = strtok (NULL, delimiters);
			key= strdup (token);

			rem (th, key, M);

			free ( key);

		} else if (strcmp (token, "find") == 0) {
			token = strtok (NULL, delimiters);
			key= strdup (token);

			int ok = find (th, key, M);
			
			if (ok)
				fprintf (outfile, "True\n");
			else
				fprintf (outfile, "False\n");
	
			free (key);

		} else if (strcmp (token, "print") == 0) {
			printHT (th, outfile);

		} else if (strcmp (token, "print_bucket") == 0) {
			char *index;

			token = strtok (NULL, delimiters);
			index = strdup (token);

			print_bucket (th, index, outfile);
			
			free (index);
		} else
			printf ("Unknown command:\"%s\".\n", token);
  }


// distrugerea tabelei hash
	for(i = 0; i < th->M; i++)
		for (aux = th->v[i]; aux != NULL;) {
			k = aux;
			aux = aux->urm;
			free (k->name);
			free (k->ip);
			free (k);
		}

	free (th->v);
	free (th);
	th = NULL;

	fclose (infile);
	fclose (outfile);

	return 0;	
}
