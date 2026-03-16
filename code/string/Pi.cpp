// Pi Array (Prefix Function for KMP)

// p[i] stores the length of the longest proper prefix of the substring s[0...i] 
// that is also a suffix of this substring.
//
// Time Complexity: O(N)
// Space Complexity: O(N)

vector<int> Pi(string &s) {
    int n = (int) s.size();
    vector<int> p(n, 0);

    for (int i = 1, j = 0; i < n; i++) {
        while(j > 0 and s[j] != s[i]) j = p[j-1];
        if (s[j] == s[i]) j++;
        p[i] = j;
    }

    return p;
}