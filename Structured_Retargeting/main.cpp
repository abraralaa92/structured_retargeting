#include "node.h"
#include <iostream>

using namespace std;
////*************READ ME!!!*************////

/////////////////////////////////////////////////////////////////////
//I have Four options to apply retargeting on: (one NW, one TDR), (one NW, all_TDRs), (set_of_NWs, one TDR), (set_of_NWs, all_TDRs)
//1- (one NW, one TDR) by calling: tes_NW (with retarget_all_TDRs set to (false))
//2- (one NW, all_TDRs) by calling: tes_NW (with retarget_all_TDRs set to (true))
//3- (set_of_NWs, one TDR) Apply retargeting on a "Bunch of benchmarks", generated from specific NW topology, at once, with only one "target_reg" accessing per each by calling: test_generated_Benchmarks (with retarget_all_TDRs set to (false))
//4- (set_of_NWs, all_TDRs) by calling: test_generated_Benchmarks (with retarget_all_TDRs set to (true))
/////////////////////////////////////////////////////////////////////
//***Please*** Don't forget that there are also three options to print NW statistics:
//1- statistics about target_reg's retargeting.
//2- statistics about the retargeting of each NW TDR separately.
//3- statistics about the whole NW readings.
/////////////////////////////////////////////////////////////////////


//int main(int argc, char** argv) //https://www.tutorialspoint.com/cprogramming/c_command_line_arguments.htmargc 
int main()
{
	printf("**********Structured Retargeting**********\n");
	printf("Please Enter the required parameters with the following order: \n");
	printf("{type_of_retargeting, path, initial_conf, write_read, target_all_TDRs} \n\n");
	node target_reg;
	
	char *inputs[] = { "", "2", "./../../Benchmarks_Generator/Generator/Benchmarks_10000_TDRs/2_children_per_SIB/2_hierarchies/", "0", "1", "1"}; 
	char** argv = inputs;

	//Set of examples on different way of retargeting *options*:
	//First set associated with the retargeting of "generated" networks from NW_genrator

	//*for MDFS on specific TDR
	//target_reg.test_NW(true, NW_state, path1 + "NW_clauses.txt", path1 + "NW_smv.pdl", path1 + "NW_SDG.txt", children_per_SIB, hierarchy_level, false, false);
	//target_reg.test_NW(true, NW_state, path2 + "NW_clauses.txt", path2 + "NW_smv.pdl", path2 + "NW_SDG.txt", 0, 0, false, false);

	//*for MPO_DFS "without" conflict resolving on specific TDR
	//target_reg.test_NW(false, NW_state, path + "NW_clauses.txt", path + "NW_smv.pdl", path + "NW_SDG.txt", children_per_SIB, hierarchy_level, false, false);

	//*for MPO_DFS *with* conflict resolving
	//target_reg.NW_statistics(2, 2, NW_state, 3, false);

	//*To gather statistics for the "all the TDRs" in the same NW, while applying MDFS
	//target_reg.test_NW(true, NW_state, path + "NW_clauses.txt", path + "NW_smv.pdl", path + "NW_SDG.txt", children_per_SIB, hierarchy_level, false, true);

	//*To gather statistics for the the whole NW and apply both type of retargeting 
	//target_reg.test_NW(true, NW_state, path + "NW_clauses.txt", path + "NW_smv.pdl", path + "NW_SDG.txt", children_per_SIB, hierarchy_level, true, true);
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////

	//Second set of examples on the retargeting of "real" networks
	//target_reg.test_NW(true, NW_state, path + "NW_clauses.txt", path + "NW_smv.pdl", path + "NW_SDG.txt", 0, 0, false, false);

	//apply MDFS on a NW with only ("NW_clauses", "NW_smv.pdl") files.
	//target_reg.test_NW(true, NW_state, path2 + "NW_clauses.txt", path2 + "NW_smv.pdl");

	//*To gather statistics for the "all the TDRs" in the same NW, while applying MPO_DFS

	target_reg.test_NW(stoi(argv[1]), argv[2], argv[3], stoi(argv[4]), stoi(argv[5]));

	cin.get();
	return 0; 
}