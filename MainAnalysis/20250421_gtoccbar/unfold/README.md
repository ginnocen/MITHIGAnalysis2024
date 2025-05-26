RooUnfold: ROOT Unfolding Framework
===
RooUnfold is a framework for unfolding (AKA "deconvolution" or "unsmearing"). It currently implements six methods:

  - iterative ("Bayesian"; as proposed by [D'Agostini](https://arxiv.org/abs/1010.0632));
  - singular value decomposition ([SVD](https://arxiv.org/abs/hep-ph/9509307), as proposed by Höcker and Kartvelishvili and implemented in [TSVDUnfold](https://root.cern.ch/doc/master/classTSVDUnfold.html));
  - bin-by-bin (simple correction factors);
  - an interface to the [TUnfold](https://root.cern.ch/doc/master/classTUnfold.html) method developed by [Stefan Schmitt](https://arxiv.org/abs/1205.6201);
  - simple inversion of the response matrix without regularisation; and
  - [iterative dynamically stabilized unfolding (IDS)](https://arxiv.org/abs/0907.3791) by Bogdan Malaescu, implemented by Chris Meyer.

It can be used from the [ROOT](https://root.cern.ch/) prompt, from C++ (Cling) or Python (PyROOT) scripts, or linked against the ROOT libraries.

There is extensive documentation available online:
  - the excellent [RooUnfold tutorial](http://statisticalmethods.web.cern.ch/StatisticalMethods/unfolding/RooUnfold_01-Methods/) by Vince Croft
  - auto-generated [Doxygen class documentation](http://roounfold.web.cern.ch/hierarchy.html) for the RooUnfold package
  - RooUnfold package [README](README.md)
  - RooUnfold package [release notes](History.md)

For support in using RooUnfold, please contact the package maintainers at [roounfold-support@cern.ch](mailto:roounfold-support@cern.ch).

They are the primary author, [Tim Adye](mailto:T.J.Adye@rl.ac.uk) (or [adye](https://gitlab.cern.ch/adye)),
and/or other package maintainers, [Carsten Burgard](mailto:cburgard@cern.ch) (or [cburgard](https://gitlab.cern.ch/cburgard)), [Lydia Brenner](mailto:lbrenner@cern.ch) (or [lbrenner](https://gitlab.cern.ch/lbrenner)), and [Pim Verschuuren](mailto:pverschu@cern.ch) (or [pverschu](https://gitlab.cern.ch/pverschu)).

RooUnfold news, such as announcements of major new versions, will be reported to a mailing list,
which you can subscribe to [here](https://e-groups.cern.ch/e-groups/EgroupsSubscription.do?egroupName=roounfold-announce).

RooUnfold was written by Tim Adye, Carsten Burgard, Richard Claridge, Chris Meyer, Kerstin Tackmann, and Fergus Wilson.
To cite the RooUnfold package in a publication, you can
refer to the [RooUnfold GitLab repository](https://gitlab.cern.ch/RooUnfold/RooUnfold)
(previously [this web page](http://hepunx.rl.ac.uk/~adye/software/unfold/RooUnfold.html)) and/or the paper:

   > Tim Adye, in Proceedings of the PHYSTAT 2011 Workshop on Statistical
   > Issues Related to Discovery Claims in Search Experiments and
   > Unfolding, CERN, Geneva, Switzerland, 17–20 January 2011, edited by
   > H.B. Prosper and L. Lyons, [CERN–2011–006, pp. 313–318](https://cdsweb.cern.ch/record/1306523).

Other documents included in this package:
  - [Corrected error calculation for iterative Bayesian unfolding](https://roounfold.web.cern.ch/bayes_errors.pdf) ([source](doc/bayes_errors))
  - [PHYSTAT 2011 proceedings](https://roounfold.web.cern.ch/phystat2011_adye.pdf) ([arXiv](https://arxiv.org/abs/1105.1160), [source](doc/phystat2011))
  - [DESY 2010 Unfolding Workshop proceedings](https://roounfold.web.cern.ch/adye_tim.pdf) ([Indico](https://indico.desy.de/indico/event/3009/other-view?view=standard#2010-05-28), [source](doc/unfolding2010))


Unfolding Method
----------------

We use unfolding to remove the known effects of measurement
resolutions, systematic biases, and detection efficiency
to determine the "true" distribution.
We parametrise the measurement effects using a _response matrix_
that maps the (binned) true distribution onto the measured one.
For 1-dimensional true and measured distribution bins <b><i>T<sub>j</sub></i></b> and
<b><i>M<sub>i</sub></i></b>,
the response matrix element <b><i>R<sub>ij</sub></i></b> gives
the fraction of events from bin <b><i>T<sub>j</sub></i></b> that end up
measured in bin <b><i>M<sub>i</sub></i></b>.
The response matrix is usually determined using Monte Carlo simulation (_training_),
with the true values coming from the generator output.

The unfolding procedure reconstructs the true <b><i>T<sub>j</sub></i></b>
distribution from the measured <b><i>M<sub>i</sub></i></b> distribution,
taking into account the measurement uncertainties due to statistical fluctuations
in the finite measured sample
(without these uncertainties, the problem could be solved uniquely by inverting
the response matrix). RooUnfold provides several algorithms for solving this problem.

The _iterative_ and _SVD_ unfolding algorithms require
a _regularisation parameter_ to prevent the statistical fluctuations
being interpreted as structure in the true distribution. It is therefore necessary
to optimise this parameter for the number of bins and sample size, using
Monte Carlo samples of the same size as the data.
These samples can also be used to measure the effectiveness of the unfolding
and hence provide estimates of the systematic errors that result from the procedure
(_testing_).

Note that for this last step (in particular), it is important to
use Monte Carlo samples with truth distributions that are statistically and
systematically independent of the sample used in training
(such samples would anyway be used in a systematics analysis,
eg. using a different generator, or reweighting variations within the a-priori
uncertainties in the truth distribution).
After all, if the Monte Carlo were a perfect model of the data,
we could use the Monte Carlo truth information directly and dispense with unfolding altogether!


The bin-by-bin method assumes no migration of events between bins
(eg. resolution is much smaller than the bin size and no systematic
shifts). This is of course trivial to implement without resorting to
the RooUnfold machinery, but is included in the package to allow simple
comparison with the other methods.


Using RooUnfold
---------------

To use RooUnfold, we must first supply the response matrix object `RooUnfoldResponse`.
It can be constructed like this:

    RooUnfoldResponse response (nbins, x_lo, x_hi);

or, if different truth and measured binning is required,

    RooUnfoldResponse response (nbins_measured, x_lo_measured, x_hi_measured,
                                nbins_true,     x_lo_true,     x_hi_true);

or, if different binning is required,

    RooUnfoldResponse response (hist_measured, hist_truth);


In that last case, `hist_measured` and `hist_truth` are used to specify
the dimensions of the distributions (the histogram contents are not used here), eg.
for 2D or 3D distributions or non-uniform binning.


This `RooUnfoldResponse` object is often most easily filled by looping over the training sample
and calling `response.Fill(x_measured,x_true)` or,
for events that were not measured due to detection inefficiency,
`response.Miss(x_true)`

    if (measurement_ok)
      response.Fill (x_measured, x_true);
    else
      response.Miss (x_true);

Alternatively, the response matrix can be constructed from a pre-existing `TH2D`
2-dimensional histogram (with truth and measured distribution `TH1D` histograms
for normalisation).

This `response` object can be passed directly to the unfolding
object, or written to a ROOT file for use at a later stage (search for
[`examples/RooUnfoldTest.cxx`](examples/RooUnfoldTest.cxx)'s
`stage` parameter for an example of how to do this).

To do the unfolding (either to try different regularisation parameters,
for testing, or for real data), create a `RooUnfold` object and pass it the
test / measured distribution (as a histogram) and the `response` object.

    RooUnfoldBayes    unfold (&response, hist_measured, iterations);

or

    RooUnfoldSvd      unfold (&response, hist_measured, kterm);

or

    RooUnfoldBinByBin unfold (&response, hist_measured);

`hist_measured` is a pointer to a `TH1D` (or `TH2D` for the 2D case)
histogram of the measured distribution (it should have the same binning as the response matrix).
The classes `RooUnfoldBayes`, `RooUnfoldSvd`, and `RooUnfoldBinByBin`
all inherit from `RooUnfold` and implement the different algorithms.
The integer `iterations` (for RooUnfoldBayes) or `kterm` (RooUnfoldSvd) is
the regularisation parameter. (Note that RooUnfoldSvd's `kterm` parameter
is also known as `tau` in the code. That usage is incompatible with the literature,
so we adopt ***k*** here.)

The reconstructed truth distribution (with errors) can be
obtained with the `Hreco()` method.

    TH1D* hist_reco= (TH1D*) unfold.Hreco();

The result can also be obtained as as a `TVectorD` with
full `TMatrixD` covariance matrix.

Multi-dimensional distributions can also be unfolded, though this does not
work for the SVD method, and the interface is rather clumsy (we hope to improve this).

See the [class documentation](https://roounfold.web.cern.ch/hierarchy.html) for details of the
`RooUnfold` and `RooUnfoldResponse` public methods.

A very simple example of RooUnfold's use is given
in [`examples/RooUnfoldExample.cxx`](examples/RooUnfoldExample.cxx).
More complete tests, using different toy MC distributions, are
in [`examples/RooUnfoldTest.cxx`](examples/RooUnfoldTest.cxx)
and [`examples/RooUnfoldTest2D.cxx`](examples/RooUnfoldTest2D.cxx).


Choice of Regularisation Parameter
----------------------------------

The regularisation parameter determines the relative weight placed on the
data, compared to the training sample truth. Both RooUnfoldBayes and RooUnfoldSvd
take integer regularisation parameters, with small values favouring the MC truth
and larger values favouring the data.

### RooUnfoldBayes Regularisation

For RooUnfoldBayes, the regularisation parameter specifies the number
of iterations, starting with the training sample truth (iterations=0).
You should choose a small integer greater than 0 (we use 4 in the examples).
Since only a few iterations are needed, a reasonable performance can
usually be obtained without fine-tuning the parameter.

The optimal regularisation parameter
can be selected by finding the largest value up to which the errors
remain reasonable (ie. do not become much larger than previous values).
This will give the smallest systematic errors (reconstructed distribution
least biased by the training truth), without too-large statistical errors.
Since the statistical errors grow quite rapidly beyond this point,
but the systematic bias changes quite slowly below it, it can be prudent
to reduce the regularisation parameter a little below this optimal point.

### RooUnfoldSvd Regularisation

For RooUnfoldSvd, the unfolding is something
like a Fourier expansion in "result to be obtained" vs "MC truth input".
Low frequencies are assumed to be systematic differences between the training MC
and the data, which should be retained in the output. High frequencies are assumed to
arise from statistical fluctuations in data and unfortunately get
numerically enhanced without proper regularization.
Choosing the regularization parameter, ***k*** (`kterm`),
effectively determines up to
which frequencies the terms in the expansion are kept. (Actually,
this is not quite true, we don't use a hard cut-off but a smooth one.)

The correct choice of ***k*** is of particular importance for
the SVD method. A too-small value will bias the unfolding result
towards the MC truth input, a too-large value will give a result that
is dominated by unphysically enhanced statistical fluctuations.
This needs to be tuned for any given distribution, number of bins,
and approximate sample size &mdash; with ***k*** between 2 and the number of bins.
(Using ***k***=1 means you get only the training truth input as result without
any corrections. You basically regularise away any differences,
and only keep the leading term which is, by construction, the MC truth input.)

[Höcker and Kartvelishvili's paper](http://arxiv.org/abs/hep-ph/9509307)
(section 7) describes how to choose the optimum value for ***k***.

Installing the Library
----------------------

This library can be installed via `pip` with version `10+`:

```
pip install git+https://gitlab.cern.ch/RooUnfold/RooUnfold@master
```

Building the Library
--------------------

Make sure that [ROOT](https://root.cern.ch/) is set up correctly:
the `$ROOTSYS` environment variable
should point to the top-level ROOT directory, `$ROOTSYS/bin` should be
in your `$PATH`, and `$ROOTSYS/lib` should be in
your library path (`$LD_LIBRARY_PATH` on most Unix systems).
This can be achieved using ROOT's
`thisroot.sh` (or `thisroot.csh`) setup script.
Eg. to use the CERN CVMFS ROOT version 6.18/02
on 64-bit CentOS7 from a bash shell:

    . /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.18.02/x86_64-centos7-gcc48-opt/bin/thisroot.sh

For further details, consult the [ROOT "Getting Started" documentation](https://root.cern.ch/root/htmldoc/guides/users-guide/ROOTUsersGuide.html#getting-started),
or your local system administrator.


Check out RooUnfold

    git clone https://gitlab.cern.ch/RooUnfold/RooUnfold.git
    cd RooUnfold

Use GNU `make`. Just type

    make

to build the RooUnfold shared library.

Alternatively, you can use `cmake`

    mkdir build
    cd build
    cmake ..
    make -j4


Running
-------

In an interactive ROOT shell, [`RooUnfoldExample.cxx`](examples/RooUnfoldExample.cxx) makes a simple test of RooUnfold.

    % root
    root [0] .L examples/RooUnfoldExample.cxx
    root [1] RooUnfoldExample()

You can also use python

    % python examples/RooUnfoldExample.py

The example programs can also be run from the shell command line.
More involved tests, allowing different toy MC PDFs to be used for training
and testing, can be found in [`examples/RooUnfoldTest.cxx`](examples/RooUnfoldTest.cxx)
(which uses test class [`RooUnfoldTestHarness`](examples/RooUnfoldTestHarness.icc)).

    % make bin
    % ./RooUnfoldTest

The output appears in `RooUnfoldTest.ps`.

You can specify parameters for `RooUnfoldTest` (either as an argument to the routine,
or as parameters to the program), eg.

    % root
    root [2] RooUnfoldTest("method=2 ftestx=3")

or

    % ./RooUnfoldTest method=2 ftestx=3

Use `RooUnfoldTest -h` or `RooUnfoldTest("-h")`to [list all the parameters and their defaults](https://roounfold.web.cern.ch/RooUnfoldTest.txt).

`method` specifies the unfolding algorithm to use:

    0	  no unfolding (output copied from measured input)
    1	  Bayes
    2	  SVD
    3	  bin-by-bin
    4	  TUnfold
    5	  matrix inversion

`ftrainx` and `ftestx` specify training and test PDFs:

    0	  flat distribution
    1	  top-hat distribution over mean &#177; 3 x width
    2	  Gaussian
    3	  Double-sided exponential decay
    4	  Breit-Wigner
    5	  Double Breit-Wigner, peaking at mean<tt>-</tt>width and mean<tt>+</tt>width
    6	  exponential
    7	  Gaussian resonance on an exponential background

The centre and width scale of these signal PDFs can be specified with the `mtrainx` and `wtrainx`
(and `mtestx` and `wtestx`) parameters respectively.
A flat background of fraction `btrainx` (and `btestx`) is added.
Detector effects are modelled with a variable shift
(`xbias` in the centre, dropping to 0 near the edges),
a smearing of `xsmear` bins,
as well as a variable efficiency (between `effxlo` at `xlo` and `effxhi` at `xhi`).

For 2D and 3D examples look at [`RooUnfoldTest2D`](https://roounfold.web.cern.ch/RooUnfoldTest2D.txt) and [`RooUnfoldTest3D`](https://roounfold.web.cern.ch/RooUnfoldTest3D.txt).

### Testing without RooFit

The test programs, [`examples/RooUnfoldTest.cxx`](examples/RooUnfoldTest.cxx), [`examples/RooUnfoldTest2D.cxx`](examples/RooUnfoldTest2D.cxx), and [`examples/RooUnfoldTest3D.cxx`](examples/RooUnfoldTest3D.cxx) use
RooFit to generate the toy distributions.
(RooFit is not required to use the RooUnfold classes from another program,
eg. [`examples/RooUnfoldExample.cxx`](examples/RooUnfoldExample.cxx)).
Hand-coded alternatives are provided if ROOT was not build with RooFit enabled
(eg. `--enable-roofit` not specified).
This version generates peaked signal events over their full
range, so may have a fewer events within the range than requested.

To disable the use of RooFit, `#define NOROOFIT` before loading `RooUnfoldTest*.cxx`

    root [0] #define NOROOFIT 1
    root [1] .x examples/RooUnfoldTest.cxx

For the stand-alone case, use

    % make bin NOROOFIT=1

to build (this is the default if RooFit is not available).


Limitations
-----------

  - RooUnfoldSvd does not work correctly for multi-dimensional distributions (gives a warning).
  - Using the histogram under/overflow bins (`UseOverflow()` setting) only works for 1D histograms.


Authors
---

  - Tim Adye (RAL, <T.J.Adye@rl.ac.uk>) is the principal RooUnfold developer and contact.
  - Carsten Burgard (NIKHEF) migrated the RooUnfold package to GitLab and added CMake build.
  - Richard Claridge (RAL) implemented the TUnfold interface, matrix inversion, and bin-by-bin algorithms as well as the error and parameter analysis frameworks.
  - Chris Meyer (Indiana) implemented the IDS algorithm.
  - Kerstin Tackmann (LBNL) wrote the SVD algorithm for the unfolding of the hadronic mass spectrum in B→Xulν.
  - Fergus Wilson (RAL) wrote an initial implementation of the iterative Bayesian algorithm.

References
----------

### Overview

1. T. Adye, [_RooUnfold: unfolding framework and algorithms_](http://hepunx.rl.ac.uk/~adye/talks/080520-unfold.pdf),
   presentations to the [Oxford](http://www.physics.ox.ac.uk/atlas/) and [RAL](http://hepunx.rl.ac.uk/atlas/ral/physics/meetings/) ATLAS Groups, May 2008.
   This includes a brief introduction to unfolding but the description of RooUnfold is now dated
   (see [below](#phystat2011-RooUnfold) for a more recent description).
2. G. Cowan, [_A Survey of Unfolding Methods for Particle Physics_](http://www.ippp.dur.ac.uk/Workshops/02/statistics/proceedings/cowan.ps),
   [Proc. Advanced Statistical Techniques in Particle Physics, Durham (2002)](http://www.ippp.dur.ac.uk/Workshops/02/statistics/).
3. G. Cowan, [_Statistical Data Analysis_](http://www.pp.rhul.ac.uk/~cowan/sda/),
   Oxford University Press (1998), Chapter 11: _Unfolding_
4. R. Barlow, [SLUO Lectures on Numerical Methods in HEP (2000)](http://www-group.slac.stanford.edu/sluo/Lectures/Stat_Lectures.html),
   [Lecture 9: _Unfolding_](http://www-group.slac.stanford.edu/sluo/Lectures/stat_lecture_files/sluolec9.pdf)

### Techniques

1. <a name="phystat2011-RooUnfold">T. Adye</a>, [_Unfolding algorithms and tests using RooUnfold_](http://arxiv.org/abs/1105.1160), write-up of a
   [presentatation](http://indico.cern.ch/getFile.py/access?contribId=72&amp;sessionId=1&amp;resId=1&amp;materialId=slides&amp;confId=107747) at the
   [_PHYSTAT 2011 workshop on unfolding and deconvolution_](http://indico.cern.ch/conferenceOtherViews.py?view=standard&amp;confId=107747#2011-01-20), CERN (January 2011).\
   Previously [presented](https://indico.desy.de/materialDisplay.py?contribId=8&amp;materialId=slides&amp;confId=3009)
   at the [_Alliance Workshop on Unfolding and Data Correction_](https://indico.desy.de/conferenceTimeTable.py?confId=3009), DESY (May 2010).
2. A. H&#246;cker and [V. Kartvelishvili](http://www.lancs.ac.uk/depts/physics/staff/kartvelishvili.html),
   _SVD Approach to Data Unfolding_, [NIM A **372** (1996) 469](http://arxiv.org/abs/hep-ph/9509307).
3. BaBar Collaboration, B. Aubert _et al._, _Study of b&rarr;ul&nu; Decays on the Recoil of Fully Reconstructed B Mesons and Determination of |V<sub>ub</sub>|_,
   [hep-ex/0408068](http://arxiv.org/abs/hep-ex/0408068), contribution to the [32nd International Conference on High Energy Physics](http://ichep04.ihep.ac.cn/) (ICHEP'04, Beijing 2004).
4. [G. D'Agostini](http://www.roma1.infn.it/~dagos/prob+stat.html), _A multidimensional unfolding method based on Bayes theorem_,
   [NIM A **362** (1995) 487](http://www.slac.stanford.edu/spires/find/hep/www?j=NUIMA,A362,487).
5.
   a. [V. Blobel](http://www.desy.de/~blobel/), [_Unfolding Methods in High Energy Physics Experiments_](http://doc.cern.ch/yellowrep/1985/1985-009/p88.pdf),
      DESY **84-118** (1984) and [Proc. 8th CERN School of Computing (Aiguablava, Spain, 1984), CERN **85-09**](http://cdsweb.cern.ch/search?sysno=000072556cer).\
   b. V. Blobel, [_An unfolding method for high energy physics experiments_](http://www.ippp.dur.ac.uk/Workshops/02/statistics/proceedings/blobel2.ps),
      [Proc. Advanced Statistical Techniques in Particle Physics, Durham (2002)](http://www.ippp.dur.ac.uk/Workshops/02/statistics/).
6. [_Alliance Workshop on Unfolding and Data Correction_](https://indico.desy.de/conferenceTimeTable.py?confId=3009), DESY (May 2010).
7. [PHYSTAT 2011 workshop on unfolding and deconvolution](http://indico.cern.ch/conferenceOtherViews.py?view=standard&amp;confId=107747#2011-01-20), CERN (January 2011).

Further Development
---
RooUnfold is currently being maintained by Tim Adye, Carsten Burgard, Lydia Brenner, and Pim Verschuuren. The latest version can always be found at the [CERN GitLab page](https://gitlab.cern.ch/RooUnfold/RooUnfold).

