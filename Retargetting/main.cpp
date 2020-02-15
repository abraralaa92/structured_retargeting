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


int main()
{
	unsigned int children_per_SIB = 2;
	unsigned int hierarchy_level = 2;
	string folder_location = "./Benchmarks_" + to_string(max_no_TDRs) + "_TDRs/";
	string path1 = folder_location + to_string(children_per_SIB) + "_children_per_SIB/" + to_string(hierarchy_level) + "_hierarchies/";
	string path2 = "./UpperBound_Benchmarks/N4/";
	string path3 = "./UpperBound_Benchmarks/corner_cases/4_CSUs/";

	string NW_state = "0"; 
	node target_reg;

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

	//*To loop on all SIB_based benchmarks and upply MPO_DFS
	//target_reg.test_generated_Benchmarks(false, NW_state, true, folder_location, "NW_clauses.txt", "NW_smv.pdl", "", false, false); //True for SIB_based, False: for mux based
	////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////
	
	//Second set of examples on the retargeting of "real" networks
	//target_reg.test_NW(true, NW_state, path + "NW_clauses.txt", path + "NW_smv.pdl", path + "NW_SDG.txt", 0, 0, false, false);
	
	//apply MDFS on a NW with only ("NW_clauses", "NW_smv.pdl") files.
	target_reg.test_NW(true, NW_state, path2 + "NW_clauses.txt", path2 + "NW_smv.pdl");

	//*To compute CSU upperbound
	measurement CSU_upperBound = target_reg.get_CSU_upperBound(NW_state, path2 + "NW_clauses.txt", 0, 0);
	
	cout << "The CSU upper bound for this networks = " << CSU_upperBound.name << " CSUs\n";
	cout << "CSU upper bound generation time = " << CSU_upperBound.reading << " sec\n";
	
	cin.get();
	return 0; 
}