template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(int i) {
    // TODO
    if (i <= 0) i = 100;      // Use the provided size; default to 100 if invalid
    M  = i;
    ht = new LinkedList<Record>*[M];     // layer 1: array of pointers
    for (int idx = 0; idx < M; ++idx) {
        ht[idx] = new LinkedList<Record>();  // layer 2: each bucket is a dynamic LinkedList<Record>
    }
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(const HashTableOpen<Key, Val>& copyObj)
: M(0), ht(nullptr) {
    copy(copyObj);
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>& HashTableOpen<Key, Val>::operator=
(const HashTableOpen<Key, Val>& rightObj) {
    if (this != &rightObj) {
        clear();
        copy(rightObj);
    }
    return *this;
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::~HashTableOpen() {
    // TODO
    if (ht != nullptr) {
        for (int i = 0; i < M; ++i) {
            delete ht[i];     // delete each dynamic LinkedList
            ht[i] = nullptr;
        }
        delete[] ht;          // delete the dynamic array of pointers
        ht = nullptr;
    }
    M = 0;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::clear() {
    for (int i = 0; i < M; i++) {
        ht[i]->clear();
    }
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::copy(const HashTableOpen<Key, Val>& copyObj) {
    if (M != copyObj.M) {
        LinkedList<Record>** tmp = new LinkedList<Record>*[copyObj.M];
        if (M < copyObj.M) {
            for (int i = 0; i < M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = M; i < copyObj.M; i++) {
                tmp[i] = new LinkedList<Record>;
            }
        }
        else if (M > copyObj.M) {
            for (int i = 0; i < copyObj.M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = copyObj.M; i < M; i++) {
                delete ht[i];
            }
        }

        M = copyObj.M;
        delete[] ht;
        ht = tmp;
    }

    LinkedList<Record>* myList      = nullptr;
    LinkedList<Record>* copyList    = nullptr;
    int                 myListLen   = 0;
    int                 copyListLen = 0;

    for (int i = 0; i < M; i++) {
        myList      = ht[i];
        myListLen   = myList->getLength();
        copyList    = copyObj.ht[i];
        copyListLen = copyList->getLength();

        if (myListLen < copyListLen) {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = myListLen; j < copyListLen; j++) {
                if (myList->isEmpty()) {
                    myList->append(copyList->getElement(j));
                }
                else {
                    myList->insert(0, copyList->getElement(j));
                }
            }
        }
        else if (myListLen > copyListLen) {
            for (int j = 0; j < copyListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = copyListLen; j < myListLen; j++) {
                myList->remove(j);
            }
        }
        else {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
        }
    }
}

template <typename Key, typename Val>
Val HashTableOpen<Key, Val>::find(const Key& k) const {
    // TODO
     int slot = hash(k);                         // fold-hash to get bucket index
    LinkedList<Record>* bucket = ht[slot];
    int len = bucket->getLength();

    for (int i = 0; i < len; ++i) {
        Record r = bucket->getElement(i);
        if (r.k == k) {
            return r.v;                        // return value of first matching record
        }
    }
    throw string("find: error, key not found"); // required error behavior
}


template <typename Key, typename Val>
int HashTableOpen<Key, Val>::hash(const Key& k) const {
    // how long should each fold be
    // changing this means you should also change the reinterpeted data type
    const int FOLD_LEN = 4;

    // if the fold length is 4, then must treat the string as unsigned numbers
    unsigned* ikey = (unsigned*) k.c_str();

    // calculate how many folds there are
    int ilen = k.length() / FOLD_LEN;

    // accumulator
    unsigned sum = 0;

    // for each fold, now treated as a number, add it to the running total
    for (int i = 0; i < ilen; i++) {
        sum += ikey[i];
    }

    // calculate how many leftover characters there are
    int extra = k.length() - ilen * FOLD_LEN;

    // create a temporary array that will hold those extra characters
    char temp[FOLD_LEN];

    // clear out that array with a 0 value
    ikey    = (unsigned*) temp;
    ikey[0] = 0;

    // copy the extra characters over
    for (int i = 0; i < extra; i++) {
        temp[i] = k[ilen * FOLD_LEN + i];
    }

    // add these final characters as a number to the running total
    sum += ikey[0];

    // calculate the slot position
    int slot = sum % M;

    // display the hashing information
    cout << k << "\thashes to slot " << slot << endl;

    // return the valid slot position
    return slot;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::insert(const Key& k, const Val& v) {
    // TODO
     int slot = hash(k);
    LinkedList<Record>* bucket = ht[slot];

    if (bucket->isEmpty()) {
        bucket->append(Record(k, v));
    } else {
        bucket->insert(0, Record(k, v));
    }
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::remove(const Key& k) {
    // TODO
     int slot = hash(k);
    LinkedList<Record>* bucket = ht[slot];
    int len = bucket->getLength();

    for (int i = 0; i < len; ++i) {
        Record r = bucket->getElement(i);
        if (r.k == k) {
            bucket->remove(i);           
            return;
        }
    }
    throw string("remove: error, key not found");
}

template <typename Key, typename Val>
int HashTableOpen<Key, Val>::size() const {
    // TODO
     int total = 0;
    for (int i = 0; i < M; ++i) {
        total += ht[i]->getLength();     
    }
    return total;
}
