#include "string"
#include <vector>
#include <chrono>

const unsigned int max_no_timeFrames = 10; //GLOBAL_CONST_VAR, resonable amount number of time frames for optimal retargeting discovering.
using namespace std;

enum input_file_type
{
	Selection_clauses,
	SDG_connections,
	smv_pdl,
	Selection_clauses_and_NW_TDRs
};

struct selection
{
	string reg_id;
	string selectionClause;
	unsigned int reg_len;
	unsigned int reset_val;

	//Empty constructor 
	selection() = default;

	//default constructor
	selection(const string& a, const string &b, const unsigned int& c, const unsigned int& d)
		: reg_id(a), selectionClause(b), reg_len(c), reset_val(d) {}

	//copy constructor
	selection(const selection& x)
		: reg_id(x.reg_id), selectionClause(x.selectionClause), reg_len(x.reg_len), reset_val(x.reset_val)
	{
		printf("selection copied!!\n");
	}

	// assignment operator modifies object, therefore non-const
	inline void operator= (selection const &j)
	{
		this->reg_id = j.reg_id;
		this->selectionClause = j.selectionClause;
		this->reg_len = j.reg_len;
		this->reset_val = j.reset_val;
	}
};

struct connection
{
	string reg_id;
	vector<string> next_OrderedNodes;
	string selection_control;
	unsigned int length;

	connection() = default;
	connection(const string& a, const vector<string> &b, const string& c, const unsigned int& d)
		: reg_id(a), next_OrderedNodes(b), selection_control(c), length(d) {}
	connection(const connection& x)
		: reg_id(x.reg_id), next_OrderedNodes(x.next_OrderedNodes), selection_control(x.selection_control)
	{
		printf("connection copied!!\n");
	}
	~connection()
	{
		std::vector<string>().swap(next_OrderedNodes);
	}
};

struct reg
{
	string reg_id;
	int state; //int state (and not unsigned int state): first (int not bool) to consider SCBs with more than one control bit, second (signed int not unsigned int) to consider SCBs with initially "unknown state/uninitialized state" corresponding to (-1) state.
	unsigned int length = 1; //this default value can be needed in case of SCT vector, where I'm interested only in setting the values of (reg_id, state).

	reg() = default;
	reg(const string& a, const unsigned int &b, const unsigned int& c)
		: reg_id(a), state(b), length(c) {}
	reg(const string& a, const unsigned int &b)
		: reg_id(a), state(b) {}
	reg(const reg& x)
		: reg_id(x.reg_id), state(x.state), length(x.length)
	{
		//printf("reg copied!!\n");
	}

	operator string() { return reg_id; } //used as an implicit struct type conversion operator from struct_reg to string; to be used through the 'Alphanumeric Sorting Algorithm'
	bool operator==(reg const &i) //will be needed inside the 'find' method, otherwise this error will be generated "binary '==': no operator found which takes a left hand operand of type 'reg' .. "
	{
		return (reg_id == i.reg_id);
	}
};

struct SDG_node
{
	string reg_id;
	string selection_control;
	unsigned int length;
	unsigned int state;
	//here I will use vector instead of dynamic array using new operator(x.next = new SDG_node[]), because the number of links (connected nodes) is unknown specially for (Br and SIB.SI nodes) where the prev pointer could points to multiple nodes and their count value couldn't be known until the whole network is traversed, so using the dynamic array I may had to resize this array multiple times which may affect on performance that's why I switched to vector STD container
	vector <SDG_node*> next; // vector <Type*> vect; vect will be on stack and Type* will be on heap.//object is allocated on the stack, however it contains a pointer which is binded to an array allocated in the heap!, Type* pointers are allocated on heap, because amount of the pointers can change dynamically and vect in this case is allocated on stack, because you defined it as a local stack variable.
	vector <SDG_node*> prev; //vector because I may have multiple next childrens and prev parents, any "INLINE" SCB has multiple nexts, any Br_node has multiple prevs  

	SDG_node(const string& a, const string &b, const unsigned int& c, const unsigned int &d, const vector <SDG_node*> &e, const vector <SDG_node*> &f)
		: reg_id(a), selection_control(b), length(c), state(d), next(e), prev(f) {}
	SDG_node(const SDG_node& x)
		: reg_id(x.reg_id), selection_control(x.selection_control), length(x.length), state(x.state), next(x.next), prev(x.prev)
	{
		printf("SDG_node copied!!\n");
	}
	~SDG_node()
	{
		std::vector<SDG_node*>().swap(next);
		std::vector<SDG_node*>().swap(prev);
	}
};

class node; //I need to use a forward declaration; so that the compiler knows that "node" is a class when it first sees it inside the struct.
struct node_address
{
	string reg_id;
	node* address;

	node_address(const string& a, node* b)
		: reg_id(a), address(b) {}
	node_address(const node_address& x)
		: reg_id(x.reg_id), address(x.address)
	{
		//printf("node_address copied!!\n");
	}

	bool operator==(string const &i)
	{
		return (reg_id == i);
	}
	operator string() { return reg_id; } //used as an implicit type conversion operator from node to string; to be used through the 'Alphanumeric Sorting Algorithm'
};

struct SDGnode_address
{
	string reg_id;
	SDG_node* address; // the length of the pointer depends on the address bus width of the processor. On a 32-bit processor, a pointer would take 4 memory bytes, on a 64-bit processor, a pointer would take 8 memory bytes.

	SDGnode_address(const string& a, SDG_node* b)
		: reg_id(a), address(b) {}
	SDGnode_address(const SDGnode_address& x)
		: reg_id(x.reg_id), address(x.address)
	{
		printf("SDGnode_address copied!!\n");
	}

	bool operator==(string const &i) //will be needed inside the 'find' method, otherwise this error will be generated "binary '==': no operator found which takes a left hand operand of type 'SDG_node' .. "
	{
		return (reg_id == i);
	}
};

struct NWElement_statistics
{
	string reg_id;
	unsigned int n_CSU_cycles; //#CSU/access
	unsigned int AccessTime; //Access time (CC)
	double execution_time;
	unsigned int SCT_size;
	bool type_of_retargeting;  //True for MDFS, False for MPO_DFS
	vector<vector<string>> n_tracedNodes; //Traced nodes
	vector<vector<node_address>> retargeting_vectors;

	NWElement_statistics() = default;
	NWElement_statistics(const string& a)
		: reg_id(a), n_CSU_cycles(0), AccessTime(0), execution_time(0), SCT_size(0), type_of_retargeting(true), n_tracedNodes(NULL), retargeting_vectors(NULL)
	{
		n_tracedNodes.reserve(max_no_timeFrames);
		retargeting_vectors.reserve(max_no_timeFrames);
	}
	NWElement_statistics(const string& a, const unsigned int &b, const unsigned int &c, const double &d, unsigned int e, const bool &f, const vector <vector<string>> &g)
		: reg_id(a), n_CSU_cycles(b), AccessTime(c), execution_time(d), SCT_size(e), type_of_retargeting(f), n_tracedNodes(g) {}
	NWElement_statistics(const NWElement_statistics& x)
		: reg_id(x.reg_id), n_CSU_cycles(x.n_CSU_cycles), AccessTime(x.AccessTime), execution_time(x.execution_time), SCT_size(x.SCT_size), type_of_retargeting(x.type_of_retargeting), n_tracedNodes(x.n_tracedNodes), retargeting_vectors(x.retargeting_vectors)
	{
		printf("NWElement_statistics copied!!\n");
	}
	~NWElement_statistics()
	{
		for (size_t i = 0, e = n_tracedNodes.size(); i < e; i++)
			std::vector<string>().swap(n_tracedNodes[i]);
		std::vector<vector<string>>().swap(n_tracedNodes);

		for (size_t i = 0, e = retargeting_vectors.size(); i < e; i++)
			std::vector<node_address>().swap(retargeting_vectors[i]);
		std::vector<vector<node_address>>().swap(retargeting_vectors);
	}
	void update_record(const unsigned int &n_CSUs, const unsigned int &n_cycles, const double &time, unsigned int SCT_size, const bool &retargeting_method, const vector <vector<string>> &n_tracedNodes)
	{
		n_CSU_cycles = n_CSUs;
		AccessTime = n_cycles;
		execution_time = time;
		this->SCT_size = SCT_size;
		type_of_retargeting = retargeting_method;
		this->n_tracedNodes = n_tracedNodes;
	}
};

class node
{
	string reg_id;
	string selection_clause;
	unsigned int depth;
	//int state; //No need for that variable I can get the current state of SCB/SIB nodes from the SCBState vector directly because other nodes like Regs don't have a state value anyway, so no need to dedicate a variable for that 
	unsigned int reset;
	int sat_value;			 //sat_value: is the node's satisfying value. We initialize it with unknown value (-1) until we build the SCT and set the correct satisfying value for each node using the selection clauses. (int not unsigned int) because sometimes we want to set this value to the (any state) value or (-2), like in case of CSU_upperBound where want to know the max_no_CSUs for applying (any taret congiurauion). (-2) <--> any state.
	bool active;
	bool satisfied;
	unsigned int noOfChildren;
	node** ptrToChildren;	 //pointer to (array of pointers) to nodes, each pointer points to one of the node's children/first child //this is a dynamic memory allocation where I need to keep nodes always in memory unless it have been deleted explicitly, I used (pointer to pointer) and not (pointer to array of node's children) directly because this will save a lot of memory where the size of (array of pointers) < size of (array of nodes), also I have to keep track of the node's address in order to be freed later from heap, and finally, this is because the node itself is saved in heap using the "new" keyword by a pointer pointing to its location in heap, so ptrToChildren is a pointer to (array) of pointers each of which is pointing to one of the node's children.
	unsigned int weight;	 //This attribute is used in tree sorting and it represents the node's distance from the leaf nodes (and not from the root node like depth attribute) 
	unsigned int length;	 //represent the number of bits of TDR, SCB or SIB

public:
	node();//this is used for dynamic memory allocation like "new node[x.noOfChildren];"
	node(string &reg_id, unsigned int depth, int sat_value); // I have to put them this order (to be suitable with line 85 in node.cpp)
	node(const node& other, unsigned int depth, int sat_value); //copy constructor will be used in 'vector.push_back(x)' for memory reallocation and different temporary copy operations. pass you parameters by reference to const.

	~node();
	void reserve_vectors_memory(); //Very Important method used to minimze the number of copied objects and to adjust the capacity of used vectors for better performance, default that I'm retargeting only one TDR, applying only one type of retargeting, and the NW_SDG input file is not available.
	void reset_system();
	void delete_vectors(); //used to reset all the variables in the program to be used in other TDR retargeting.
	void clear_vectors();//this method is used only to reset network by "Clearing" vectors and not "Freeing" it, where I need to use them again in accessing other NW_TDRs, I want to set and adjust capacity of all vectors only once and clear it after each accessing until all NW_TDRs are traversed by that time I can call (reset_system()) to free its associated memory
	void reset_vectors(); //used to clear the vector when you want to continue to use the object for other purposes.You can clear the object and then fill it again.If you destroy the object, it is gone and you would have to construct a new one.As indicated you never call the destructor explicitly.The destructor is called when a stack allocated object goes out of scope at the end of a block or when you explicitly delete the object.
	void reset_counters();
	void free_memory(); //destroy all the SRs' trees created in heap
	void destroy_tree(node &x);
	void destroy_SDG(SDG_node &x);

	void printSDG(const SDG_node &x);
	void generate_AP(); //Active Scan Path
	bool is_satisfied(const node &x);
	bool is_active(const node &x);
	void update_nodes_states(node &root); //update the state of both (active, satisfy) each node in the tree after each tree traversal
	void load_initial_NWstate();
	void set_current_NWstate();  //this is to set the resulting state of the previous retargeting step as the current network state.
	void load_current_NWstate();  //this is to start retargeting from the resulting state of the previous retargeting step.
	void update_SCBStates();
	void update_SCBStates(vector<reg> &p);
	void MDFS_conflict_resolution();
	void MPO_DFS_conflict_resolution();
	void MDFS(node &x);
	void MPO_DFS(node &x);
	void MDFS_writeModel(node &x);
	void MPO_DFS_writeModel(node &x);
	int check_id_existence(vector<node_address> & p, const string& reg_id); //the difference between (check_node_existence()) and (check_id_existence()) is that in check_id: we check the existence of (id) in some passed vector, while for check_node: we check the existence of (id, sat_value) in the vector, so we enable sat_value checking also.
	int check_node_existence(vector<node_address> & p, const node &x); //return pos of node if it is exist, else return -1
	int check_SDGnode_existence(vector<SDGnode_address> & p, const string& reg_id);
	int check_reg_existence(vector<reg> & p, const string& reg_id);
	void print_AP();
	void generate_unVisitedNodes_SCT();
	void satisfy_node();
	void sort_tree(node &x);
	void check_conflict(node &x);
	bool is_SCB_SIB(string& reg_id);

	unsigned int Inst_AccessTime(); //with unit of (TCKs)
	void adjust_scanPath_len(); //True for increment, False for decrement
	double get_avg(char x); //A->access time, C->CSU cycles, E->execution time
	double get_max(char x);
	void build_SDG(SDG_node& x);
	void generate_SDG_AP(SDG_node &x); //Active Scan Path though the SDG
	void generate_AVs();
	void generate_AVs(vector<node_address> &AN);
	void print_AVs();			//this method to print current time frame AVs.
	void print_AVs_total();		//while this method to print the AVs generated output vectors for "all" the time frames.
	void print_output_retargeting_vectors(const string& output_file);
	void sort_AVs_and_validate_model(); //retargeting output vector would be sorted based on the NW SDG connection, also this method check is that the output vector is already accessed by the ASP generated from NW SDG.
	void print_AVs_sorted();
	size_t get_selection_clause_index(const string& reg_id);
	unsigned int get_SCT_size(node& x);
	unsigned int get_n_tracednodes_per_TReg(unsigned int TDR_index);

	void load_NW(const string& NW_clauses_file, const string& NW_smv_pdl_file, const string& NW_SDG_file = "", bool retarget_all_TDRs = false); //used to load input files and adjust the capacity of all vectors used, //load (NW_clause, NW_SDG, target_reg, initial_config, reset_state), (bool retarget_all_TDRs) to check if I want to gather statistics for specific reg_id or for the whole/all network. retargeting_upperBound: (true) for loading the NW for "Retargeting", (false) for loading the NW for "CSU upper bound computations".
	void load_input_files(const string& input_file, input_file_type option);
	void initialize_NW(); //this is to always start pattern retargeting from the user-predefined "initial" state.
	void generate_SCT(node &x); //pass by reference to keep the node's attributes (specially Children) saved after the return call, this method could generate both(targetReg_SCT and NW_SCT) based on the selection clause of the target_reg.
	void generate_output_retargeting_vectors();
	void apply_retargeting(bool structured_retargeting_method, bool write_read, const string& output_file); //structured_retargeting_*method*: (True) for MDFS, (False) for MPO_DFS without Conflict checking
	void test_NW(unsigned int method_of_retargeting, const string& path, const string& NW_state, bool write_read, bool retarget_all_TDRs = false); //unsigned int method_of_retargeting: !=(1) for retargeting using (MDFS), (2) for retargeting using (MPO_DFS), (3) for applying both ways of retargeting (MDFS, MPO_DFS)
	void print_NWstatistics(bool target_statistics, unsigned int index = 0); //true: for "target_reg" statistics, false: for "whole NW" statistics.

	//this group of commands is correlated with Conflict checking/ Tree sorting. The implementation of these sections could be enhanced by using *pointer to function* concept, where it has the same implementation like each othere with difference in only (one) line of could which could be handeled by pointer to function
	void generate_SCT_withConflictChecking(node &x);
	void apply_retargeting_withConflictChecking(bool structured_retargeting_method, const string& output_file);
	void initialize_NW_withConflictChecking(); //for enable_conflictChecking: this attribute is used if we want to check for coflict between nodes while generating the tree(this can be done by calling 'generate_tree_withConflictChecking' and not the regular one 'generate_tree')

	//operator string() { return reg_id; } //used as an implicit type conversion operator from node to string; to be used through the 'Alphanumeric Sorting Algorithm'
	bool operator== (const node& x);  //The function "std::find" uses operator== to compare the individual elements to val -- line 289.
	node& operator= (const node& x);  //t2 = t1;  // calls assignment operator, same as "t2.operator=(t1);"
									  //Test t3 = t1;  // calls copy constructor, same as "Test t3(t1);"
	friend int myPow(int x, int p);
};

inline int myPow(int x, int p) {
	if (p == 0) return 1;
	if (p == 1) return x;
	return x * myPow(x, p - 1);
}