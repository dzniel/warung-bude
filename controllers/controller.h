#include "../models/model.h"
#include <math.h>

// Untuk menampilkan OS
const char* detectOS() {
  #ifdef _WIN32
  return "Windows 32-bit";
  #elif _WIN64
  return "Windows 64-bit";
  #elif __APPLE__ || __MACH__
  return "Mac OSX";
  #elif __linux__
  return "Linux";
  #elif __FreeBSD__
  return "FreeBSD";
  #elif __unix || __unix__
  return "Unix";
  #else
  return "Other";
  #endif
}

// Untuk membuat node dish baru
Dish *createDish(char *name, int price, int qty) {
  Dish *temp = (Dish *)malloc(sizeof(Dish));
  strcpy(temp->name, name);
  temp->price = price;
  temp->qty = qty;
  temp->next = temp->prev = NULL;
  return temp;
}

// Untuk membuat node customer baru
Customer *createCustomer(char *name) {
  Customer *temp = (Customer *)malloc(sizeof(Customer));
  strcpy(temp->name, name);
  temp->prevCust = temp->nextCust = NULL;
  return temp;
}

// Untuk membuat node order baru
Order *createOrder(char *name, int qty, int price) {
  Order *temp = (Order *)malloc(sizeof(Order));
  strcpy(temp->name, name);
  temp->qty = qty;
  temp->price = price;
  temp->nextOrder = temp->prevOrder = NULL;
  return temp;
}

// Untuk push node dish ke head
void pushHead(char *name, int price, int qty) {
  Dish *temp = createDish(name, price, qty);
  if(!head) {
    head = tail = temp;
  } else {
    temp->next = head;
    head->prev = temp;
    head = temp;
  }
}

// Untuk push node dish ke tail
void pushTail(char *name, int price, int qty) {
  Dish *temp = createDish(name, price, qty);
  if(!head) {
    head = tail = temp;
  } else {
    tail->next = temp;
    temp->prev = tail;
    tail = temp;
  }
}

// Priority queue atau push sekaligus ngesort sesuai quantity dari node dish
void pushMid(char *name, int price, int qty) {
  Dish *temp = createDish(name, price, qty);
  if(!head) {
    head = tail = temp;
  } else if(qty <= head->qty) {
    pushHead(name, price, qty);
  } else if(qty >= tail->qty) {
    pushTail(name, price, qty);
  } else {
    curr = head;
    while(qty > curr->qty) {
      curr = curr->next;
    }
    temp->prev = curr->prev;
    temp->next = curr;
    curr->prev->next = temp;
    curr->prev = temp;
  }
}

// Untuk membebaskan node head
void popHead() {
  if(!head) {
    return;
  } else if(head == tail) {
    head = tail = NULL;
    free(head);
  } else {
    Dish *temp = head;
    head = head->next;
    head->prev = NULL;
    free(temp);
  }
}

// Untuk membebaskan node tail
void popTail() {
  if(!head) {
    return;
  } else if(head == tail) {
    head = tail = NULL;
    free(head);
  } else {
    Dish *temp = tail;
    tail = tail->prev;
    tail->next = NULL;
    free(temp);
  }
}

// Untuk membebaskan node berdasarkan quantitynya
void popMid(int qty) {
  if(!head) {
    return;
  } else if(qty == head->qty) {
    popHead();
  } else if(qty == tail->qty) {
    popTail();
  } else {
    curr = head;
    while(curr->next && qty != curr->qty) {
    
      curr = curr->next;
    }
    curr->prev->next = curr->next;
    curr->next->prev = curr->prev;
    curr = NULL;
    free(curr);
  }
}

// Hash function
unsigned long DJB2(char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % 26;
}

// Insert customer ke index tertentu pada table sesuai hasil hash
void insert(char *name) {
  int index = DJB2(name);
  Customer *temp = createCustomer(name);
  if(!headCust[index]) {
    headCust[index] = tailCust[index] = temp;
  } else {
    tailCust[index]->nextCust = temp;
    temp->prevCust = tailCust[index];
    tailCust[index] = temp;
  }
}

// Untuk search customer apakah exist atau tidak
bool search(char *name) {
  int index = DJB2(name);
  if(headCust[index]) {    
    currCust = headCust[index];
    while(currCust && name != currCust->name) {
      if(strcmp(currCust->name, name) == 0) {
        return true;
      }
      currCust = currCust->nextCust;
    }
    if(!currCust->nextCust) {
      return false;
    }
  }
  return false;
}

// Untuk mendapatkan posisi customer pada index tertentu dan pada node tertentu
Customer *getNode(char *name) {
  int index = DJB2(name);
  if(headCust[index]) {
    currCust = headCust[index];
    while(currCust) {
      if(!strcmp(currCust->name, name) == 0) {
        return currCust;
      }
      currCust = currCust->nextCust;
    }
  }
  return NULL;
}

// Masukin order ke list order yang ada pada list masing-masing customer
void insertOrder(Customer *temp, char *name, int qty, int price) {
    Order *newOrder = createOrder(name, qty, price);
    if(!temp->headOrder) {
      temp->headOrder = temp->tailOrder = newOrder;
    } else {
      currOrder = temp->headOrder;
      while(currOrder) {
        currOrder = currOrder->nextOrder;
      }
      currOrder->nextOrder = newOrder;
      newOrder->prevOrder = currOrder;
      temp->tailOrder = newOrder;
    }
}

// Untuk menghapus seluruh order setelah dicheckout sama customer
void popOrder(int index) {
  if(!headCust[index]->headOrder) {
    return;
  } else if(headCust[index]->headOrder == headCust[index]->tailOrder) {
    headCust[index]->headOrder = headCust[index]->tailOrder = NULL;
    free(headCust[index]->headOrder);
  } else {
    currOrder = headCust[index]->headOrder;
    while(currOrder) {
      currOrder = currOrder->nextOrder;
    }
    Order *temp = currOrder;
    headCust[index]->tailOrder = currOrder->prevOrder;
    currOrder->prevOrder->nextOrder = NULL;
    free(temp);
  }
}

// Untuk menampilkan semua nama customer pada table
void view() {
  for(int i = 0; i < 26; i++) {
    if(!headCust[i]) {
      continue;
    } else if(headCust[i]) {
      printf("%d. ", i);
      currCust = headCust[i];
      while(currCust) {
        if(currCust)
        printf("%s -> ", currCust->name);
          currCust = currCust->nextCust;
      }
    }
    puts("NULL");
  }
}

bool displayDish() {
  if(!head) {
    puts("No menu added yet."); getchar();
    return false;
  }
  puts("Bude's Menu");
  puts("=============================================");
  printf("%-5s %-19s %-9s %-10s\n", "No.", "Name", "Quantity", "Price");
  int i = 1;
  curr = head;
  while(curr) {
    printf("%-5d %-19s %03d %-5s Rp%-8d\n", i, curr->name, curr->qty, " ", curr->price);
    i++;
    curr = curr->next;
  }
  puts("=============================================");
  return true;
}

// Display semua yang perlu dicheckout
void displayPayment(int index) {
  int i = 1, totalPrice = 0;
  currOrder = headCust[index]->headOrder;
  printf("%s\n", headCust[index]->name);
  while(currOrder) {
    printf("[%d] %s x%d\n", i, currOrder->name, currOrder->qty);
    totalPrice += currOrder->price;
    currOrder = currOrder->nextOrder;
  }
  printf("Total: Rp%d\n", totalPrice);
}


// Clear screen
void cls() {
  system("cls||clear");
}