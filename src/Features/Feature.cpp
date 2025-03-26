#include "Feature.h"


Feature::Feature(Geometry *geometry): geometry(geometry){

}

Feature::~Feature(){
    delete geometry;
}