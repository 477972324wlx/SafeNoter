
#include <random>
 
using namespace std;

static constexpr char Printable[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static constexpr char Illegal[]   = "\\/;\'\"$#|><";
namespace util{ 
    string makeRandStr(int sz, bool printable)
    {
        string ret;
        ret.resize(sz);
        std::mt19937 rng(std::random_device{}());
        for (int i = 0; i < sz; ++i)
        {
            if (printable)
            {
                uint32_t x = rng() % (sizeof(Printable) - 1);
                ret[i] = Printable[x];
            } 
            else 
            {
                ret[i] = rng() % 0xFF;
            }
        }
    
        return ret;
    }

    bool checkFilename(const string filename){
        for (auto & ch : filename){
            for (auto& illegal_char : Illegal){
                if (ch == illegal_char){
                    return false;
                }
            }
        }
        return true;
    }

}