# OldAndNewSchoolCpplus
Exec time optimization by going through rvalue references :
The code realizes the timing of standard operations of creation, copy, and assignment for some large objects.
First one object only handles the Rule of Three as if computed by oldest version of C++  : destructor, copy constructor, copy assignment operator.
Second object on the other hand implements the so-called Rule of Five  : as above, and moreover move constructor and move assignment operator.
The program does some operations for the first one object, then it does the same for the second one.
And in the mean time, it displays the computation timing for both : we may see this way that the « old school » object is doing things
much slowly than the « new school » one, and compare such timing though.

As in the result just below, where for the first object, we got some ten seconds as for the total computation time,
while it is less than one second and an half for the second object to do the same things.

-------------------------------------------------------
Old  School C++      C++98
current computation elapsed time : 0s for regular constructor
current computation elapsed time : 0.432738s for copy constructor (lvalue in input)
current computation elapsed time : 0.543671s for move constructor (rvalue in input)
current computation elapsed time : 0.546662s for assignment operator (lvalue in input)
current computation elapsed time : 7.55863s for move assignment operator (rvalue in input)
current computation elapsed time : 9.80103s for Total computation


-------------------------------------------------------
New School C++       C++11
current computation elapsed time : 0s for regular constructor
current computation elapsed time : 0.556656s for copy constructor (lvalue in input)
current computation elapsed time : 0.0227251s for move constructor (rvalue in input)
current computation elapsed time : 0.55966s for assignment operator (lvalue in input)
current computation elapsed time : 0.162899s for move assignment operator (rvalue in input)
current computation elapsed time : 1.30594s for Total computation



