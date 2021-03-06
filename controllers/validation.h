#include <string.h>

// Memastikan dish name semua lowercase
bool isValidDishName(char *str) {
  for(int i = 0; str[i] != '\0'; i++) {
    if(str[i] == ' ') {
      continue;
    } else if(!(str[i] >= 'a' && str[i] <= 'z')) {
      return false;
    }
  }
  return true;
}

// Memastikan price ada di range 1000 sampai 50000
bool isValidPrice(int price) {
  return (price >= 1000 && price <= 50000) ? true : false;
}

// Memastikan quantity ada di range 1 sampai 999
bool isValidQty(int qty) {
  return (qty >= 1 && qty <= 999) ? true : false;
}

// Memastikan dish exist di dish list
bool isValidDish(char *str) {
  curr = head;
  while(curr) {
    if(strcmp(curr->name, str) == 0) {
      return true;
    } 
    curr = curr->next;
  }
  return false;
}

// Memastikan nama customer tidak memiliki spasi dan juga unique berpatokan dari customer list
bool isValidCustName(char *name) {
  for(int i = 0; name[i] != '\0'; i++) {
    if(name[i] == ' ') {
      return false;
    }
  }
  int index = DJB2(name);
  currCust = headCust[index];
  while(currCust) {
    if(!strcmp(currCust->name, name)) {
      return false;
    }
    currCust = currCust->nextCust;
  }
  return true;
}

// Memastikan nama yang diinput tidak mengandung spasi dan symbol selain alfabet
bool isValidSearchName(char *str) {
  for(int i = 0; str[i] != '\0'; i++) {
    if(!(str[i] >= 'a' && str[i] <= 'z') && !(str[i] >= 'A' && str[i] <= 'Z')) {
      return false;
    }
    if(str[i] == ' ') {
      return false;
    }
  }
  return true;
}

// Memastikan jumlah yang diinput tidak melebihi jumlah yang ada
bool isValidAmount(int amount, int n) {
  return (amount >= 1 || amount <= n) ? true : false;
}

// Memastikan dish dan quantity yang diminta tersedia pada list
bool isValidOrder(char *str) {
  bool dish = false, quantity = false;
  int len = 0;
  for(int i = 0; str[i] != '\0'; i++) {
    len++;
  }
  int qty = 0;
  for(int i = len - 1; str[i] != 'x'; i--) {
    qty += str[i] - '0';
    if(str[i - 1] != 'x') {
      qty *= 10;
    }
  }
  for(int i = len - 1; i >= 0; i--) {
    if(str[i] == ' ') {
      str[i] = '\0';
      break;
    }
  }
  curr = head;
  while(curr) {
    if(!strcmp(curr->name, str)) {
      dish = true; 
      break;
    }
    curr = curr->next;
  }
  if(qty > 0 || qty <= curr->qty) {
    quantity = true;
  }
  if(dish && quantity) {
    return true;
  }
  return false;
}

// Memastikan index pada table berisi customer setidaknya 1
bool isValidIndex(int index) {
  return headCust[index] ? true : false;
}