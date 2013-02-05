/*
 * AbstractFunction.h
 *
 *  Created on: Feb 2, 2013
 *      Author: ndp
 */

#ifndef ABSTRACTFUNCTION_H_
#define ABSTRACTFUNCTION_H_

#include <iostream>
#include "expr.h"

using std::endl;

#include "BaseType.h"

namespace libdap {


class AbstractFunction {

private:
    string name;
    string description;
    string usage;

    libdap::bool_func d_bool_func;
    libdap::btp_func  d_btp_func;
    libdap::proj_func d_proj_func;

public:
    AbstractFunction();
	virtual ~AbstractFunction();

	string getName(){ return name; }
	void setName(string n){ name = n; }

	string getDescriptionString(){ return description; }
	void setDescriptionString(string desc){ description = desc; }

	bool canOperateOn(DDS &dds) { return true; }


	void setFunction(bool_func bf){
		d_bool_func = bf;
		d_btp_func  = 0;
		d_proj_func = 0;
	}

	void setFunction(d_btp_func btp){
		d_bool_func = 0;
		d_btp_func  = btp;
		d_proj_func = 0;
	}

	void setFunction(d_proj_func pf){
		d_bool_func = 0;
		d_btp_func  = 0;
		d_proj_func = pf;
	}

	bool_func get_bool_func(){ return d_bool_func; }
	btp_func  get_btp_func() { return d_btp_func;  }
	proj_func get_proj_func(){ return d_proj_func; }




};

} /* namespace libdap */
#endif /* ABSTRACTFUNCTION_H_ */