#include "STTPremiseDialogView.h"
#include "ExpressionButton.h"
#include <stdexcept>
#include <gtkmm.h>

/*
This file called when the 'Add Premise' button is pushed
*/

void PremDialog::show(){
	this->premiseText->set_text("");
    int retval = premDialog->run();
}	

void PremDialog::cancel_clicked(){
	premDialog->hide();
}

void PremDialog::accept_clicked(){
	//Read in the premise that the user wants to add
	std::string prem_exp = this->premiseText->get_text();
	int i = this->model->getNumPremises();
	try{ // Try to parse the premise into expressions and atomic values
		this->model->addPremise(prem_exp);
		ShortTruthTables::ParsedExpression* new_exp = this->model->getPremise(i);
		Gtk::Grid* prem_grid = Gtk::manage(new Gtk::Grid());
    	Gtk::Box* premises_box;
	//Add this premise to the UI
    	refBuilder->get_widget("premise_box", premises_box);
    	premises_box->pack_start(*prem_grid, true, true);
    	premises_box->show();
    	std::string inorder = new_exp->getInOrderExp();
		for(int j = 0; j < inorder.size(); j++){
			if(new_exp->expressionAtPosition(j) == NULL){
				Gtk::Label* temp = Gtk::manage(new Gtk::Label(std::string(1,inorder[j])));
				temp->show();
				prem_grid->add(*temp);
			}else{ //Add buttons to the UI so that truth values can be assigned
				ExpressionButton* temp = Gtk::manage(new ExpressionButton(new_exp->expressionAtPosition(j), prem_grid, this->model));
				temp->set_exp_num_and_pos(i, j);
				temp->show();
				prem_grid->add(*temp);
				if(new_exp->isTopLevel(new_exp->expressionAtPosition(j))){ //If this is a new premise, start by labelling it as True
					Gtk::Label* true_label = Gtk::manage(new Gtk::Label("T"));
					true_label->show();
					prem_grid->attach_next_to(*true_label, *temp, Gtk::POS_BOTTOM ,1,1);

				}
				if(inorder[j] == '<'){
					j+=2;
				}
				else if(inorder[j] == '-'){
					j+=1;
				}

			}
		}

	//Show all UI changes
		prem_grid->show();
		premises_box->show();
		premDialog->hide();
	}catch(std::invalid_argument e){
		this->premiseText->set_text("Poorly Formatted Expression");
	}
}
