# OldAndNewSchoolCpplus
Exec time optimization by going through rvalue references. 
The code realizes the timing of standard operations of creation, copy, and assignment for some large objects.
First one object only handles the Rule of Three as if computed by oldest version of C++  : destructor, copy constructor, copy assignment operator.
Second object on the other hand implements the so-called Rule of Five  : as before, moreover move constructor and move assignment operator.
The program does some operations for the first one object, then it does the same for the second one.
And in the mean time, it displays the computation timing for both : we may see this way that the « old school » object is doing things
much slowly than the « new school » one, and compare such timing though.

How coding has been written is in no way a pure showcase of what should be done, it has been implemented on the fly in order to show evidence
of how the C++ compilator makes things happen. No header files, no prototypes, only one « main.cpp » source file that contains it all, declarations and definitions.
Two classes of object are defined, the old and new school ones, as described above. 

There is one template function « process() » which template parameter is the type of object - old or new school one - that is handled by this very process. 
This way, such very same process is applied on one object and another, validating timing comparisons so on.

There is also another template function « createHolder() » with object type specified as well, that should instantiate the current object 
and return such, thus as a temporary variable. Actually, C++ compilator is so powerfull that it is enacting a RVO, means a « Return Value Otimization »
on its own, and there is no difference in instance assignment process, whatever the current object has or doesn't have a move constructor, which is not what we want. 
In order to mislead the compilator and avoid the RVO, the function is creating two local instances, retrieving only one of them depending on some external factor 
(on this one, it's the current day date, peer / odd).

In the result just below, for the first object, total computation time is some ten seconds long,
while it is less than one second and an half for the second object to do the same things.

-------------------------------------------------------\
Old  School C++      C++98\
current computation elapsed time : 0s for regular constructor\
current computation elapsed time : 0.432738s for copy constructor (lvalue in input)\
current computation elapsed time : 0.543671s for move constructor (rvalue in input)\
current computation elapsed time : 0.546662s for assignment operator (lvalue in input)\
current computation elapsed time : 7.55863s for move assignment operator (rvalue in input)\
current computation elapsed time : 9.80103s for Total computation\
\
\
-------------------------------------------------------\
New School C++       C++11\
current computation elapsed time : 0s for regular constructor\
current computation elapsed time : 0.556656s for copy constructor (lvalue in input)\
current computation elapsed time : 0.0227251s for move constructor (rvalue in input)\
current computation elapsed time : 0.55966s for assignment operator (lvalue in input)\
current computation elapsed time : 0.162899s for move assignment operator (rvalue in input)\
current computation elapsed time : 1.30594s for Total computation



