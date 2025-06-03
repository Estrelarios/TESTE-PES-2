#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;


// classe do nó
class node
{
private:

  node *esq, *dir;  // ponteiro esquerdo e direito
  int chave;  // dado
  int altura; // altura


public:
  node(int chave) // construtor
  {
    this->chave = chave;
    esq = NULL;
    dir = NULL;
    altura = 1;
  }

  int getChave() { return chave; }   // método de acesso a chave
  node* getEsq() { return esq; }   // método de acesso ao nó esquerdo
  node* getDir() { return dir; }  // método de acesso ao nó direito
  void setEsq(node *no) { esq = no;}  // método de definição do nó esquerdo
  void setDir(node *no) { dir = no; } // método de definição do nó direito
  void setChave(int chave) { this->chave = chave; } // método de definição da chave do nó
  void setAltura(){ // método de definição da altura do nó 
    int alturaEsq = esq ? esq->getAltura() : 0;
    int alturaDir = dir ? dir->getAltura() : 0;
    altura = 1 + max(alturaEsq, alturaDir);
  }
  int getAltura(){ return this->altura; } // método de acesso a altura do nó

};





// classe da árvore 
class arvoreAVL
{
private:
  node *raiz; // raiz da árvore
public:
  arvoreAVL() { raiz = NULL; }  // construtor
  void inserir(int chave);  // método de inserção na árvore
  node* inserirAux(node *no, int chave);  // método de inserção na árvore
  node *getRaiz() // método de acesso a raiz da arvore
  {
    return raiz;
  }
  void setRaiz(node *no){ this->raiz = no; }  // método de definição da raiz 
  void emOrdem(); // percurso em ordem
  void emOrdemAux(node* no, int altura);  // percurso em ordem
  void remover(int chave);  // método de remoção de nó
  node* remocao(node *no, int chave); // método de remoção de nó
  node* sucessor(node* no); // método auxiliar de busca de sucessor
  node* antecessor(node *no); // método auxiliar de busca de antecessor
  node* rotacaoEsq(node *no); // método de rotação esquerda
  node* rotacaoDir(node *no); // método de rotação direita
  node* rotacaoDuploEsq(node *no); // método de rotação Left-right
  node* rotacaoDuploDir(node *no);	// método de rotação Right-left

};

void arvoreAVL::remover(int chave){
  raiz = remocao(raiz, chave);
}

node* arvoreAVL::remocao(node* no, int chave) {
  // Caso base: árvore nula
  if (no == NULL)
    return no;

  // Verifica se valor é menor
  if (chave < no->getChave()) {
    no->setEsq(remocao(no->getEsq(), chave));
  } 
  // Verifica se valor é maior
  else if (chave > no->getChave()) {
    no->setDir(remocao(no->getDir(), chave));
  } 
  else {
    // Caso 01: Nó folha
    if (no->getEsq() == NULL && no->getDir() == NULL) {
      delete no;
      return NULL;
    }
    // Caso 02: Nó com um único filho
    if (no->getEsq() == NULL) {
      node* temp = no->getDir();
      delete no;
      return temp;
    } 
    else if (no->getDir() == NULL) {
      node* temp = no->getEsq();
      delete no;
      return temp;
    }

    // Caso 03: Nó com dois filhos
    node* s = antecessor(no);
    no->setChave(s->getChave());
    no->setEsq(remocao(no->getEsq(), s->getChave()));
  }

  // Atualizar altura do nó
  int alturaEsq = no->getEsq() ? no->getEsq()->getAltura() : 0;
  int alturaDir = no->getDir() ? no->getDir()->getAltura() : 0;
  no->setAltura();

  // Recalcular o fator de balanceamento (FB)
  int FB = alturaEsq - alturaDir;

  // Rotação para corrigir desbalanceamento
  if (FB > 1) { // Nó está desbalanceado para a esquerda
    int FBEsq = no->getEsq()->getEsq() ? no->getEsq()->getEsq()->getAltura() : 0;
    int FBEsqDir = no->getEsq()->getDir() ? no->getEsq()->getDir()->getAltura() : 0;

    if (FBEsq >= FBEsqDir) {
      return rotacaoDir(no); // LL
    } else {
      no->setEsq(rotacaoEsq(no->getEsq()));
      return rotacaoDir(no); // LR
    }
  }

  if (FB < -1) { // Nó está desbalanceado para a direita
    int FBDirEsq = no->getDir()->getEsq() ? no->getDir()->getEsq()->getAltura() : 0;
    int FBDir = no->getDir()->getDir() ? no->getDir()->getDir()->getAltura() : 0;

    if (FBDir >= FBDirEsq) {
      return rotacaoEsq(no); // RR
    } else {
      no->setDir(rotacaoDir(no->getDir()));
      return rotacaoEsq(no); // RL
    }
  }

  return no;
}



node* arvoreAVL::sucessor(node* no)
{
  // sucessor, nó mais a esquerda da subárvore da direita
  //sucessor nunca tem filho esquerdo!
  no = no->getDir();
  while(no != NULL && no->getEsq() != NULL)
    no = no->getEsq();
  return no;
}

node* arvoreAVL::antecessor(node *no)
{
  //antecessor o nó mais a direita da subárvore esquerda
  no = no->getEsq();
  while(no != NULL && no->getDir() != NULL)
    no = no->getDir();
  return no;
}

void arvoreAVL::emOrdem(){
  emOrdemAux(this->getRaiz(),0);
}
  
void arvoreAVL::emOrdemAux(node* no, int altura)
{
  if (no != NULL)
  {
    emOrdemAux(no->getEsq(), altura + 1);
    cout << no->getChave() << "," <<  altura << endl;
    emOrdemAux(no->getDir(), altura + 1);
  }
}



void arvoreAVL::inserir(int chave) {

  // promove alterações pela raiz
  raiz = inserirAux(raiz, chave);
}

node* arvoreAVL::inserirAux(node* no, int chave) {
  if (no == NULL)
      return new node(chave);
  // valor é menor que o nó
  if (chave < no->getChave()) {
      no->setEsq(inserirAux(no->getEsq(), chave));
  } 
  // valor é maior que o nó
  else if (chave > no->getChave()) {
      no->setDir(inserirAux(no->getDir(), chave));
  } else {
      cout << "Esse valor já existe na árvore.\n";
      return no;
  }
  
  // Fator de balanceamento do nó
  int FB;
  no->setAltura();
  int alturaEsq = no->getEsq() ? no->getEsq()->getAltura() : 0;
  int alturaDir = no->getDir() ? no->getDir()->getAltura() : 0;
  FB = alturaEsq - alturaDir;

  // desbalanceado esquerda
  if (FB > 1 && chave < no->getEsq()->getChave()) {
      return rotacaoDir(no); // LL
  }
  // desbalanceado esquerda-direita
  if (FB > 1 && chave > no->getEsq()->getChave()) {
      no->setEsq(rotacaoEsq(no->getEsq()));
      return rotacaoDir(no); // LR
  }
  // desbalanceado direita
  if (FB < -1 && chave > no->getDir()->getChave()) {
      return rotacaoEsq(no); // RR
  }
  // desbalanceado direita-esquerda
  if (FB < -1 && chave < no->getDir()->getChave()) {
      no->setDir(rotacaoDir(no->getDir()));
      return rotacaoEsq(no); // RL
  }

  return no;
}

node* arvoreAVL::rotacaoEsq(node* no) {
    node* novaRaiz = no->getDir();
    no->setDir(novaRaiz->getEsq());
    novaRaiz->setEsq(no);

    no->setAltura();
    novaRaiz->setAltura();

    return novaRaiz;
}

node* arvoreAVL::rotacaoDir(node* no) {
  node* novaRaiz = no->getEsq();
  no->setEsq(novaRaiz->getDir());
  novaRaiz->setDir(no);

  no->setAltura();
  novaRaiz->setAltura();

  return novaRaiz;
}

node* arvoreAVL::rotacaoDuploEsq(node* no) {
  no->setDir(rotacaoDir(no->getDir()));
  return rotacaoEsq(no);
}

node* arvoreAVL::rotacaoDuploDir(node* no) {
  no->setEsq(rotacaoEsq(no->getEsq()));
  return rotacaoDir(no);
}


int main() {
    arvoreAVL avl;
    string linha;

    // Lê linha por linha da entrada padrão (stdin)
    while (getline(cin, linha)) {
        istringstream iss(linha); 
        char operacao;
        int valor;

        if (!(iss >> operacao >> valor)) continue; // Ignorar linhas inválidas
    
      
        if (operacao == 'i') {
            avl.inserir(valor); // Inserir na árvore AVL
        } else if (operacao == 'r') {
            avl.remover(valor); // Remover da árvore AVL
        }
    }

    // Escrever a árvore em ordem na saída padrão (stdout)
    avl.emOrdem();
    //cout << endl; // Para garantir que a saída seja concluída corretamente

    return 0;
}
