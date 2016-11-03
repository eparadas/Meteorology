/********************************************************************
*                                                                   *
*                                                                   *
*       Author: vparadas                                            *
*       Comments: vparadas@cc.uoi.gr                                *
*                 evangelos.paradas@cern.ch                         *
*                                                                   *
*       Description: This program reads the "linearized" files      *
*                    and produces as many as graphs                 *
*		     as the provided clusters.                      *
*                    To execute it you need first                   *
*                    to set the ROOT environement, typing root5     *
*                    Then, you are able to execute Plots.exe but    *
*                    give three arguments: a) linearized file       *
*                                          b) first column          *
*                                          c) last column           *
*                                          d) number of clusters    *
*     Results are going to be written to "c_heights.gif"            *
********************************************************************/



#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>

#include <TPad.h>
#include <TAxis.h>
#include <TImage.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>

using namespace std;

double pi()
{
	return atan(1.)*4.;
}

float arc_length(float latitude_1, float latitude_2, float longitudinal_1, float longitudinal_2)
{
	const float R = 6367.45;
	float delta_lati = latitude_1 - latitude_2;
	delta_lati *= pi()/180.;
	float delta_long = longitudinal_1 - longitudinal_2;
	delta_long *= pi()/180.;
	return 2*R*asin( sqrt( pow(sin(delta_lati/2),2) + cos(latitude_1) * cos(latitude_2) * pow(sin(delta_long/2),2) ) );
}

int main(int argc, char* argv[])
{
	TImage *img = TImage::Open("/mnt/storage_local/pkassom/Europe.GIF");  //put your own picture here
	img->SetImageQuality(TAttImage::kImgBest);
	
	ifstream inf( argv[1] );
 
	int sc = atoi( argv[2] );
	int ec = atoi( argv[3] );
	cout << "File that will be read: " << argv[1] << "\nColumns from " << sc <<" to " << ec << endl;
	
	if ( (ec-sc+1)%2 != 0 )
	{
		cout << "Total number of columns must be of the format 2n\nProgram is going to be aborted" << endl;
 		return 1;
	}
	 
	int facs(0);
	char temp;
	do{
		temp = inf.get();
		if ( temp == 'F' )
			facs++;
	}while ( temp != '\n' );
	const int clusters = atoi( argv[4] );
	
	
	char temp2[12];
	float temp_num;
	int cluster_num;
	float mean_traj_matrix[clusters], mean_from_all_traj_matrix[clusters];
	vector< vector<float> > vecx[clusters], vecy[clusters]; // these vectors will keep all values
	vector<float> tempx, tempy; // these vectors will keep only the current line
	vector<float> vec_mean_x[clusters], vec_mean_y[clusters]; // these vectors will keep all mean values
	vector<float> temp_sum_x, temp_sum_y; // these vectors will keep only the mean of current line
	
	
	cout << "Vector matrices are set" << endl;
	
//----------------Read all values--------------	
	do{
		tempx.clear();
		tempy.clear();	
		int jc(1);
		
		while ( jc <= (ec+facs+2) )
		{
			if ( jc >= sc && jc < (ec+sc+1)/2 ) // y axis
			{
				inf >> temp_num;
				tempy.push_back( temp_num );
			}
			else if ( jc >= (ec+sc+1)/2 && jc <= ec ) // x axis
			{
				inf >> temp_num;
				tempx.push_back( temp_num );
			}
			else if ( jc == ec+facs+1 ) // number of cluster
			{
				inf >> temp_num;
				cluster_num = ((int) temp_num) - 1;
			}
			else // useless entries
				inf >> temp2;
			
			jc++;
		}
		if ( cluster_num > (clusters - 1) )
			continue;
		
		vecx[cluster_num].push_back( tempx );
		vecy[cluster_num].push_back( tempy );
	}while(!(inf.eof()));
//---------------------------------------------
	cout << "All values read" << endl;
//----------Calculate mean values--------------
	for(int jk=0; jk<clusters; jk++)
	{
		temp_sum_x.clear();
		temp_sum_y.clear();
		for(unsigned int jjk=0; jjk<vecx[jk][0].size(); jjk++)
		{
			temp_sum_x.push_back(0);
			temp_sum_y.push_back(0);
			for(unsigned int jjkk=0; jjkk<vecx[jk].size(); jjkk++)
			{
				temp_sum_x.at(jjk) += vecx[jk][jjkk].at(jjk);
				temp_sum_y.at(jjk) += vecy[jk][jjkk].at(jjk);
			}
		}
		for(unsigned int jjk=0; jjk<temp_sum_x.size(); jjk++)
		{
			vec_mean_x[jk].push_back( (float) temp_sum_x.at(jjk)/vecx[jk].size() );
			vec_mean_y[jk].push_back( (float) temp_sum_y.at(jjk)/vecy[jk].size() );
		}
		//----------Calculate length of each mean arc--------------
		float temp_sum(0);
		for(unsigned int jjk=0; jjk<vec_mean_x[jk].size()-1; jjk++)
			temp_sum += arc_length( vec_mean_x[jk].at(jjk), vec_mean_x[jk].at(jjk+1), vec_mean_y[jk].at(jjk), vec_mean_y[jk].at(jjk+1) );
		 
		mean_traj_matrix[jk] = temp_sum;
		//---------------------------------------------------------
		//----------Calculate length of each trajectory------------
		double cluster_total_len(0);
		for(unsigned int jjk=0; jjk<vecx[jk].size(); jjk++)
		{
			double count_len(0);
			for(unsigned int jjkk=0; jjkk<vecx[jk][jjk].size()-1; jjkk++)
				count_len += arc_length( vecx[jk][jjk].at(jjkk), vecx[jk][jjk].at(jjkk+1), vecy[jk][jjk].at(jjkk), vecy[jk][jjk].at(jjkk+1) );
			
			cluster_total_len += count_len;
		}
		
		mean_from_all_traj_matrix[jk] = cluster_total_len/vecx[jk].size();
	}
	
//---------------------------------------------
	cout << "Mean values calculated" << endl;

	int total_graphs(0);
	for(int kk=0; kk<clusters; kk++)
		total_graphs += vecx[kk].size();
	
	TGraph *graphs[total_graphs+clusters];
	TCanvas *c_heights[clusters];
	TPad *new_pads[clusters];
	
	int d_gr(0);
	for(int kk=0; kk<clusters; kk++)
	{
		c_heights[kk] = new TCanvas("c_heights"+kk,"c_heights"+kk,1024,768);
		c_heights[kk]->cd();
		img->Draw();
		
		new_pads[kk] = new TPad("new_pads"+kk,"new_pads"+kk,0,0,1,1);
		new_pads[kk]->SetFillStyle(4000);
		new_pads[kk]->SetFrameFillStyle(4000);
		new_pads[kk]->SetTickx();
		new_pads[kk]->SetTicky();
		new_pads[kk]->SetGridx();
		new_pads[kk]->SetGridy();
		new_pads[kk]->Draw();
		new_pads[kk]->cd();
		
		img->SetConstRatio(kFALSE);
		
		for(unsigned int jj=0; jj<vecx[kk].size(); jj++)
		{
			float xaxis[vecx[kk][jj].size()];
			float yaxis[vecy[kk][jj].size()];
			
			for(unsigned int ll=0; ll<vecx[kk][jj].size(); ll++)
			{
				xaxis[ll] = vecx[kk][jj].at(ll);
				yaxis[ll] = vecy[kk][jj].at(ll);
			}
			graphs[jj+d_gr] = new TGraph(vecx[kk][jj].size(), xaxis, yaxis);
			graphs[jj+d_gr]->SetTitle("");
			graphs[jj+d_gr]->GetXaxis()->SetTitle("Latitude");
			graphs[jj+d_gr]->GetYaxis()->SetTitle("Longitude");
			graphs[jj+d_gr]->SetMarkerStyle(3);
			graphs[jj+d_gr]->SetLineColor(kBlack);
// 			graphs[jj+d_gr]->SetLineWidth(3);
			graphs[jj+d_gr]->GetXaxis()->SetLimits(-60,60);
			graphs[jj+d_gr]->GetYaxis()->SetRangeUser(0,90);
			if ( jj == 0 )
				graphs[jj+d_gr]->Draw("sameAC");
			else
				graphs[jj+d_gr]->Draw("sameC");
			
		}//for jj
		d_gr += vecx[kk].size();

		float xaxis[vec_mean_x[kk].size()];
		float yaxis[vec_mean_y[kk].size()];
		for(unsigned int jm=0; jm<vec_mean_x[kk].size(); jm++)
		{
			xaxis[jm] = vec_mean_x[kk].at(jm);
			yaxis[jm] = vec_mean_y[kk].at(jm);
		}
		
		graphs[total_graphs+kk] = new TGraph(vec_mean_x[kk].size(), xaxis, yaxis);
		graphs[total_graphs+kk]->SetMarkerStyle(1);
		graphs[total_graphs+kk]->SetMarkerColor(kRed);
		graphs[total_graphs+kk]->SetLineColor(kRed);
		graphs[total_graphs+kk]->SetLineWidth(3);
		graphs[total_graphs+kk]->GetXaxis()->SetLimits(-60,60);
		graphs[total_graphs+kk]->GetYaxis()->SetRangeUser(0,90);
		graphs[total_graphs+kk]->Draw("sameC");
		
		char c_tit[32];
		sprintf(c_tit,"c_heights%d.gif",kk+1);
		c_heights[kk]->SaveAs(c_tit);
		cout << "Canvas " << kk + 1 << endl;
	}//for kk

	cout << "  Length for mean trajectory \t Mean Length from all trajectories" << endl;
	for(int kk=0; kk<clusters; kk++)
		cout << kk+1 << "\t" << mean_traj_matrix[kk] << " km\t\t\t" << mean_from_all_traj_matrix[kk] << " km\n";
	cout << endl <<"Plots completed" << endl;
}
