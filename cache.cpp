#include <bits/stdc++.h>
using namespace std;
#define ADDRESS_LENGTH 32
int main()
{
    long long cache_size, block_size, associativity;
    string replacement_policy, write_policy;
    // Open the file
    ifstream file("./cache.config");
    if (!file.is_open())
    {
        cout << "ERROR IN OPENING FILE" << endl;
        return 0;
    }
    string line;
    getline(file, line);
    cache_size = stoll(line);
    getline(file, line);
    block_size = stoll(line);
    getline(file, line);
    associativity = stoll(line);
    getline(file, line);
    replacement_policy = line;
    getline(file, line);
    write_policy = line;
    file.close();
    long long total_blocks = cache_size / block_size;
    if (associativity == 0)
    {
        associativity = total_blocks;
    }
    long long total_sets = total_blocks / associativity;
    long long index_bits = log2(total_sets);
    long long offset_bits = log2(block_size);
    long long tag_bits = ADDRESS_LENGTH - index_bits - offset_bits;
    // tuple is used to store valid bit, tag, block and dirty bit
    vector<vector<tuple<long long, long long, long long, long long>>> cache(total_sets, vector<tuple<long long, long long, long long, long long>>(associativity, make_tuple(0, 0, 0, 0)));
    // Open one more file
    ifstream file2("./cache.access");
    if (!file2.is_open())
    {
        cout << "ERROR IN OPENING FILE" << endl;
        return 0;
    }
    // we'll assume that it is like a queue, with most recent on the front for lru
    while (getline(file2, line))
    {
        bool isWrite = (line[0] == 'W');
        line = line.substr(3);
        long long address = stoll(line, 0, 16);
        long long set_index = index_bits != 0 ? ((unsigned)(address << tag_bits)) >> (tag_bits + offset_bits) : 0; // handling fully associative
        bool hit = false;
        long long tag = (index_bits + offset_bits != ADDRESS_LENGTH) ? ((unsigned)address) >> (index_bits + offset_bits) : 0;
        long long i;
        for (i = (associativity - 1); i >= 0; i--)
        {
            if (get<0>(cache[set_index][i]) == 1 && get<1>(cache[set_index][i]) == tag)
            {
                hit = true;
                break;
            }
        }
        if (hit)
        {
            if (isWrite)
            {
                if (write_policy == "WB")
                    get<3>(cache[set_index][i]) = 1; // dirty bit is one here
            }
            if (replacement_policy == "LRU")
            {
                tuple<long long, long long, long long, long long> temp = cache[set_index][i];
                move(cache[set_index].begin() + i + 1, cache[set_index].begin() + cache[set_index].size(), cache[set_index].begin() + i);
                cache[set_index].pop_back();
                cache[set_index].push_back(temp);
            }
        }
        else
        {
            if (isWrite && write_policy == "WT")
            {
                cout << "Address: 0x" << hex << setfill('0') << setw(8) << address << ", Set: 0x" << hex << setfill('0') << setw(ceil(index_bits / 4.0)) << set_index << ", " << (hit ? "Hit" : "Miss") << ", Tag: 0x" << hex << setfill('0') << setw(ceil(tag_bits / 4.0)) << tag << endl;
                continue;
            }
            for (int j = associativity; j >= 0; j--)
            {
                if (get<0>(cache[set_index][j]) == 0)
                {
                    move(cache[set_index].begin() + 1, cache[set_index].begin() + associativity, cache[set_index].begin());
                    cache[set_index].pop_back();
                    cache[set_index].push_back(make_tuple(1, tag, 1234, 0));
                    hit = true; // using this redundant variable as a bool variable
                    break;
                }
            }
            if (hit)
            {
                hit = false;
                cout << "Address: 0x" << hex << setfill('0') << setw(8) << address << ", Set: 0x" << hex << setfill('0') << setw(ceil(index_bits / 4.0)) << set_index << ", " << (hit ? "Hit" : "Miss") << ", Tag: 0x" << hex << setfill('0') << setw(ceil(tag_bits / 4.0)) << tag << endl;
                continue;
            }
            // set is full so replace it
            if (cache[set_index].size() == associativity)
            {
                if (replacement_policy == "LRU" || replacement_policy == "FIFO")
                {
                    if (associativity == 1)
                        cache[set_index][0] = make_tuple(1, tag, 1234, 0);
                    else
                    {
                        move(cache[set_index].begin() + 1, cache[set_index].begin() + cache[set_index].size(), cache[set_index].begin());
                        cache[set_index].pop_back();
                        cache[set_index].push_back(make_tuple(1, tag, 1234, 0));
                    }
                }
                else
                    cache[set_index][rand() % associativity] = make_tuple(1, tag, 1234, 0); // store block data
            }
            else
                cache[set_index].push_back(make_tuple(1, tag, 1234, 0)); // store block data
        }
        cout << "Address: 0x" << hex << setfill('0') << setw(8) << address << ", Set: 0x" << hex << setfill('0') << setw(ceil(index_bits / 4.0)) << set_index << ", " << (hit ? "Hit" : "Miss") << ", Tag: 0x" << hex << setfill('0') << setw(ceil(tag_bits / 4.0)) << tag << endl;
    }
    file2.close();
    return 0;
}