#SCXML Designer

SCXML Designer is a QT 5 based graphical editor for designing SCXML state charts and workflows. Note that the term *workflow* in this project is used to mean a *statechart* with additional meta-data. This meta-data is held within the SCXML as comments to ensure that the output is compliant and can be used as input to external code generators and SCXML tools.

If you don't know what SCXML is, then you probably don't need this designer, but take a look at the [W3 specification](http://www.w3.org/TR/scxml/) or [Wikipedia entry](http://en.wikipedia.org/wiki/SCXML).


##Functionality
  - Create, Load and Save a workflow
  - Add a state (sort of)
  - Er, closes the form...

##TODO:
  - State and transition editing - well, it is the first commit :)
  - Allow running/stepping through a state chart, with graphical effects to show transitions taken and states active.
  - Hooks for code generators 
