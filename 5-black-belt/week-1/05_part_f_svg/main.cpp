#include <iostream>
#include <variant>
#include <vector>

#include "svg.h"

int main() {

	Svg::Document svg;

	svg.Add(
		Svg::Polyline{}
		.SetStrokeColor(Svg::Rgb{140, 198, 63})  // soft green
		.SetStrokeWidth(16)
		.SetStrokeLineCap("round")
		.AddPoint({50, 50})
		.AddPoint({250, 250})
	);

	for (const auto point : {Svg::Point{50, 50}, Svg::Point{250, 250}}) {
		svg.Add(
			Svg::Circle{}
			.SetFillColor("white")
			.SetRadius(6)
			.SetCenter(point)
		);
	}

	svg.Add(
		Svg::Text{}
		.SetPoint({50, 50})
		.SetOffset({10, -10})
		.SetFontSize(20)
		.SetFontFamily("Verdana")
		.SetFillColor("black")
		.SetData("C")
	);
	svg.Add(
		Svg::Text{}
		.SetPoint({250, 250})
		.SetOffset({10, -10})
		.SetFontSize(20)
		.SetFontFamily("Verdana")
		.SetFillColor("black")
		.SetData("C++")
	);
	svg.Add(
		Svg::Text{}
		.SetPoint({2, 30})
		.SetOffset({1, 2})
		.SetFontSize(30)
		.SetData("txt")
	);

	svg.Render(std::cout);

	return 0;
}