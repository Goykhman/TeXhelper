/*

  Set file name in the main() function.

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

class bibSort {

public:

  bibSort(string fn) {
    fileName = fn;
    currentString = "";
    isReferencing = false;
  }

  /*
    Trims part of currentString which is not commented out.
  */
  void trimCurrentString() {
    int i = currentString.find('%');
    if (i != string::npos) {
      currentString = currentString.substr(0,i);
    }
  }

  /*
    Saves current reference in the sorted list.
  */
  void saveReference(string ref) {
    int i = 0;
    while (i < ref.size() && ref[i] == ' ') {
      ++i;
    }
    ref = ref.substr(i, ref.size() - i);
    i = ref.find(' ');
    ref = ref.substr(0,i);
    //cout << ref << endl;
    if (setNames.find(ref) == setNames.end()) {
      setNames.insert(ref);
      sortedNames.push_back(ref);
    }
  }

  /*
    Returns sorted bibliography list.
  */
  vector<string> returnSorteBibliography() {
    vector<string> sortedBibliography;
    for (string name : sortedNames) {
      sortedBibliography.push_back(bibliography[name]);
    }
    return sortedBibliography;
  }

  /*
    Returns length of current reference name which starts at index i
    in the currentString.
  */
  int finishReferencing(int i) {
    if (currentString[i] == ',' || currentString[i] == '-') {
      ++i;
    }
    int j = 0;
    while (i+j < currentString.size() && currentString[i+j] != ',' && currentString[i+j] != '-' && currentString[i+j] != '}') {
      ++j;
    }
    if (i+j < currentString.size() && currentString[i+j] == '}') {
      isReferencing = false;
    }
    string ref = currentString.substr(i,j);
    saveReference(ref);
    return j;
  }

  /*
    Goes over the file prior to the bibliography , finds the references
    and sorts them in the order they appear.
  */
  void sortBibliography() {
    ifstream source(fileName);
    string str;
    while(getline(source, str)) {
      currentString = str;
      trimCurrentString();
      if (currentString.size() == 0) {
        continue;
      }
      if (currentString.find(bibStart) != string::npos) {
        break;
      }
      int i = 0;
      if (isReferencing) {
        while (isReferencing) {
          int j = finishReferencing(i);
          i += j;
        }
      }
      bool reference = true;
      while (reference) {
        i = currentString.find(citeStart, i);
        if (i != string::npos) {
          isReferencing = true;
          i += 5;
          int j = 0;
          while (isReferencing && i < currentString.size()) {
            j = finishReferencing(i);
            i += (j+1);
          }
        } else {
          reference = false;
        }
      }
    }
  }

  /*
    Goes over the file, finds the bibliography section, and populates
    the map bibliography with the references.
  */
  void composeBibliography() {
    ifstream source(fileName);
    bool inBib = false;
    string bibItem;
    string str;
    while(getline(source, str)) {
      currentString = str;
      trimCurrentString();
      if (inBib && currentString.find(endBib) != string::npos) {
        if (bibItem.size() > 0) {
          bibliography[currentName] = bibItem;
        }
        break;
      }
      if (!inBib) {
        if (currentString.find(bibStart) != string::npos)
          inBib = true;
        continue;
      }
      if (currentString.find(bibItemStart) != string::npos) {
        if (bibItem.size() > 0) {
          bibliography[currentName] = bibItem;
          bibItem = "";
        }
        int s = currentString.find(bibItemStart) + 8;
        int i = 0;
        while (s + i < currentString.size() && currentString[s+i] != '}') {
          i += 1;
        }
        string name = currentString.substr(s, i);
        currentName = name;
        bibItem += ("%\\cite{" + name + "}");
        bibItem += "\n";
        bibItem += currentString;
        bibItem += "\n";
      }
      else {
        if (currentString.size() > 0) {
          bibItem += currentString;
          bibItem += "\n";
        }
      }
    }
  }

private:
  string fileName;
  string currentString;
  bool isReferencing;
  string currentName;
  map<string,string> bibliography;
  set<string> bibItems;
  vector<string> sortedNames;
  set<string> setNames;
  /*
    Put file-specific key words here which will help the program locate
    the correct places in the text.
  */
  string bibStart = "begin{thebibliography";
  string bibItemStart = "bibitem{";
  string endBib = "end{thebibliography";
  string citeStart = "cite{";
};

int main() {
  // Specify the name of your main tex file here.
  bibSort bs("name.tex");
  bs.composeBibliography();
  bs.sortBibliography();
  /*
    Simply copy sorted bibliograpy from the command line.
  */
  vector<string> sb = bs.returnSorteBibliography();
  for (string s : sb) {
    cout << s << endl;
  }
  return 0;
}
