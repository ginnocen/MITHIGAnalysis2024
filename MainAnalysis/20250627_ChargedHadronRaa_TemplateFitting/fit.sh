#!/bin/bash
source clean.sh
TEMPLATEPATH=$(pwd)/templatetrees

./TemplateFitting \
  --Data $TEMPLATEPATH/data.root \
  --Templates $TEMPLATEPATH/template_OO.root,$TEMPLATEPATH/template_SD.root,$TEMPLATEPATH/template_DD.root \
  --TemplateNames OO,SD,DD \
  --Fractions 0.33,0.33,0.34 \
  --Branch hMult \
  --VarName "Track Multiplicity" \
  --Xmin 0 \
  --Xmax 500 \
