# DNS-server

In momentul accesarii unei pagini web este lansat un DNS QUERY catre un server 
DNS, pentru a obtine IP-ul paginii web ce se doreste a fi accesata, urmand ca 
ulterior sa se initieze o conexiune care va aduce toata informatia paginii 
respective. Cand cererea ajunge la serverul DNS, acesta va cauta in memorie o 
intrare de tipul (Name, IP), unde "Name" este adresa paginii web, iar IP este 
IP-ul asociat.

Programul va fi rulat:
make
./DNS M hash i .in hash i .out

Specificatii:
  -> key = name
  -> value = ip

Pentru aceasta am implementat o tabela hash in care pun inregistrarile.
Daca exista coliziuni, elementele vor fi adaugate in functie de restul 
impartirii sumei caracterelor ce fac parte din cheile (Key) la numarul maxim 
de liste ce pot fi stocate in hashtable (M).

In cod am implementat functia:
- put -> pune un element in lista.

- get -> intoarce valoarea corespunzatoare cheii key, iar daca key nu exista,
         intoarce NULL.

- rem -> sterge din tabela hash elementul identificat dupa key daca exista, 
         iar daca nu, nu se mai face stergerea.

- find -> caută Key in hashtable și ȋntoarce True/False.

- printTH -> afiseaza toate valorile value din hashtable; pentru fiecare 
             bucket (lista) nevid; se va afisa indicele si toate elementele 
             acestuia (Value).

- print_bucket -> afiseaza valorile value din bucketul cu indicele index, 
                  pe o singura linie. Daca lista asociata bucket-ului este vida 
                  se va afisa VIDA.
