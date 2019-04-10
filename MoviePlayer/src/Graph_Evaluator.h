#ifndef GRAPH_EVALUATOR
#define GRAPH_EVALUATOR

#include <vector>

/**
 * Graph_Evaluator.
 **/

class Graph_Evaluator
{
	friend std::ostream& operator<<( std::ostream& os, Graph_Evaluator const& f );
	
public:
	
	// Initialization
	Graph_Evaluator() {}
	Graph_Evaluator( std::vector< double > x_vals, std::vector< double > y_vals ): 
		_plot_x( x_vals ), _plot_y( y_vals ) {}
	
	// Destruction
	~Graph_Evaluator() {}
	
	// Access
	std::vector< double > const& x_vals() const { return _plot_x; }
	std::vector< double > const& y_vals() const { return _plot_y; }
	
	// Measurement
	// Comparison
	// Status report
	// Status setting
	// Cursor movement
	// Element change
	// Removal
	// Resizing
	// Transformation
	// Conversion
	// Basic operations

	double evaluate( double x ) 
	{ 
		double value = 0.;
		std::vector< double >::iterator lb, ub;
		ub = std::upper_bound( _plot_x.begin(), _plot_x.end(), abs( x ) );
	
		if ( ub == _plot_x.end() )
		{
			value = _plot_y.back();
		}
		else if ( ub == _plot_x.begin() )
		{
			value = _plot_y.front();
		}
		else
		{
			// x is in range [_plot_x.begin(), _plot_x.end()]
			lb = ub - 1;
			double weight = ( abs( x ) - *lb ) / ( *ub - *lb );
			value = weight * _plot_y[ lb - _plot_x.begin() ] + ( 1.f - weight ) * _plot_y[ ub - _plot_x.begin() ];
		}

		return value;
	}
	
	// Miscellaneous
	// Obsolete
	// Inapplicable
	
protected:
	
private:
	std::vector< double > _plot_x, _plot_y;

};	// end of class Graph_Evaluator


#endif // GRAPH_EVALUATOR

