// ------------------------------------------------------------------------------------------------------
// Does the timing of standard operations such as creation, copy, and assignment on large objects,
// once with only copy operators defined (rule of three), then with move operators as well (rule of five)
// We may compare time execution on both cases.
// ------------------------------------------------------------------------------------------------------

#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <memory>
#include <functional>

using namespace std;

// GENERAL :  les objets suivants sont RAII = "Resource Allocation Is Initialisation"
// resource allocation (or acquisition) is done during object creation (specifically initialization), by the constructor,
// while resource deallocation (release) is done during object destruction (specifically finalization), by the destructor.


// Chrono ===================================================
vector<std::chrono::system_clock::time_point> vStart;

inline void showChrono(string s) {
    auto cur = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = cur-vStart.back(); vStart.pop_back();
    std::cout << "current computation elapsed time : " << elapsed_seconds.count() << "s for " << s << "\n";
}
inline void setChrono0() { vStart.push_back(std::chrono::system_clock::now()); }
// ==========================================================

// ================================================================================
//                            OLD SCHOOL C++
// ================================================================================
class HolderOld
{
    typedef std::unique_ptr<int[]> DataPtr;

  public:

    HolderOld(int size)         // Constructor
    {
      m_data = std::make_unique<int[]>(size);
      m_size = size;
    }

    HolderOld(const HolderOld& other)     // Copy constructor
    {
      m_data = std::make_unique<int[]>(other.m_size);  // (1)
      std::copy(other.m_data.get(), other.m_data.get() + other.m_size, m_data.get());  // (2)
      m_size = other.m_size;
    }

    HolderOld& operator=(const HolderOld& other)    // Copy assignment operator
    {
      if(this == &other) return *this;  // (1)
      m_data.reset(new int[other.m_size]);  // (2)
      std::copy(other.m_data.get(), other.m_data.get() + other.m_size, m_data.get());
      m_size = other.m_size;
      return *this;  // (3)
    }

    ~HolderOld()                // Destructor
    {
      // smart means nothing to be done :-)
    }

  private:

    DataPtr m_data;
    size_t m_size;
};

// ================================================================================
//                            NEW SCHOOL C++    (C++11)
// ================================================================================
class Holder
{
    typedef std::unique_ptr<int[]> DataPtr;

  public:

    Holder(int size)  noexcept       // Constructor
    {
        m_data = std::make_unique<int[]>(size);
        m_size = size;
    }

    Holder(const Holder& other)     // Copy constructor
    {
        m_data = std::make_unique<int[]>(other.m_size);  // (1)
        std::copy(other.m_data.get(), other.m_data.get() + other.m_size, m_data.get());  // (2)
        m_size = other.m_size;
    }

    Holder(Holder&& other) noexcept    // <-- rvalue reference in input
    {
      m_data.reset(nullptr);
      m_data.swap(other.m_data);   // (1)           // Move constructor
      m_size = other.m_size;
    }

    Holder& operator=(const Holder& other)      // Copy assignment operator
    {
        if(this == &other) return *this;  // (1)
        m_data.reset(new int[other.m_size]);  // (2)
        std::copy(other.m_data.get(), other.m_data.get() + other.m_size, m_data.get());
        m_size = other.m_size;
        return *this;  // (3)
    }

    Holder& operator=(Holder&& other) noexcept    // <-- rvalue reference in input
    {
      if (this == &other) return *this;         // Move assignment operator

      m_data.reset(nullptr);
      m_data.swap(other.m_data);
      m_size = other.m_size;
      other.m_size = 0;
      return *this;
    }

    ~Holder()                // Destructor
    {
      // smart means nothing to be done :-)
    }

  private:

    DataPtr m_data;
    size_t m_size;
};

// ================================================================================
// ================================================================================
// ================================================================================
// ================================================================================

int const arraysSize0 = 1073741824;
int const arraysSize1 = 536870912;
//int const arraysSize2 = 268435456;
//int const arraysSize3 = 134217728;

template<class holder>
holder createHolder(int size)
{
// One might go ahead with only one coding line like this :
//
//    return holder(size);
//
// But so far, on both cases, "old school" and "new school",
// compilator avoids enacting such a copy/move constructor :
// - with copy of data from m_data (expensive) on old school case with simple reference &
// - with simple assignment of m_data pointer (fast) on new school case with rvalue reference &&
//
// Instead of that, the compilator  is optimizing (Return Value Optimization RVO) and is building
// the function (createHolder) result directly inside the new instance, but on the stack as usual.
//
// In order to mislead the compilator, one does so that at compile time, it is unknown
// which instance to be returned as a result : just below, say, h01 or h02 ???
//
// This way, the result is built in the function context, and is to be returned and
// copied when on the old school constructor, or moved while on the new school one.
// And difference of performances between both methods may be measured and highlighted so far.
//
// (It is possible to disable the RVO on GCC while being on Linux, with compile option -fno-elide-constructors )
//

    time_t t = time(NULL);
    tm* timePtr = localtime(&t);

    holder
           h01(size),
           h02(size);

    setChrono0();

    if (timePtr->tm_mday % 2)   // depending on day of the month, odd or even, returns h01 or h02
        return h01;
    else
        return h02;

//    return holder(size);
}

template<class holder>
void process() {

    setChrono0();

    setChrono0();
    holder h1(arraysSize1);                // regular constructor
    showChrono("regular constructor");

    setChrono0();
    holder h2(h1);                  // copy constructor (lvalue in input)
    showChrono("copy constructor (lvalue in input)");

    // setChrono0(): in the function
    holder h3(createHolder<holder>(arraysSize1)); // move constructor (rvalue in input) (1)
    showChrono("move constructor (rvalue in input)");

    setChrono0();
    h2 = h3;                        // assignment operator (lvalue in input)
    showChrono("assignment operator (lvalue in input)");

    // setChrono0()): in the function
    h2 = createHolder<holder>(arraysSize1);         // move assignment operator (rvalue in input)
    showChrono("move assignment operator (rvalue in input)");

    showChrono("Total computation");

}


int main()
{

    cout << endl << endl << "-------------------------------------------------------" << endl;
    cout << "Old  School C++      C++98" << endl;
    process<HolderOld>();

    cout << endl << endl << "-------------------------------------------------------" << endl;
    cout << "New School C++       C++11" << endl;
    process<Holder>();

    //================================================================

    cout << endl << endl << "=======================================================" << endl;
    cout << "How to move a lvalue" << endl << endl;

    // std::move change une lvalue en rvalue

    {
        Holder h1(arraysSize0);     // h1 is an lvalue
        setChrono0();
        Holder h2(h1);              // copy-constructor invoked (because of lvalue in input)
        showChrono("copy-constructor");
    }

    {
        Holder h1(arraysSize0);     // h1 is an lvalue
        setChrono0();
        Holder h2(std::move(h1));   // move-constructor invoked (because of rvalue in input)
        showChrono("move-constructor");

        // there, h1 is lost, whatever, since it's end of bloc hence, end of its data scope
    }


    return 0;
}
