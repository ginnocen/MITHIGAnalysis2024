#!/bin/bash
source clean.sh
TEMPLATEPATH=$(pwd)/templates

./TemplateFitting \
  --Data $TEMPLATEPATH/data.root \
  --Templates $TEMPLATEPATH/template_OO.root,$TEMPLATEPATH/template_SD.root,$TEMPLATEPATH/template_DD.root \
  --TemplateNames OO,SD,DD \
  --Fractions 0.33,0.33,0.33 \
  --Branches hMult,hHFEsum \
  --VarName "TrackMultiplicity","HFETSum(GeV)" \
  --Xmin 0,0 \
  --Xmax 500,100 \

