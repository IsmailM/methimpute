% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/plotting.R
\name{plotting}
\alias{plotting}
\alias{plotHistogram}
\alias{plotScatter}
\alias{plotTransitionProbs}
\alias{plotConvergence}
\alias{plotEnrichment}
\alias{plotPosteriorDistance}
\title{Methimpute plotting functions}
\usage{
plotHistogram(model, total.counts, binwidth = 1)

plotScatter(model, datapoints = 1000)

plotTransitionProbs(model)

plotConvergence(model)

plotEnrichment(model, annotation, windowsize = 100, insidewindows = 20,
  range = 1000, category.column = NULL, plot = TRUE, df.list = NULL)

plotPosteriorDistance(model, datapoints = 1e+06, binwidth = 5,
  max.coverage.y = 0, min.coverage.x = 3, xmax = 200,
  xbreaks.interval = xmax/10, cutoffs = NULL)
}
\arguments{
\item{model}{A \code{\link{methimputeBinomialHMM}} object.}

\item{total.counts}{The number of total counts for which the histogram is to be plotted.}

\item{binwidth}{The bin width for the histogram/boxplot.}

\item{datapoints}{The number of randomly selected datapoints for the plot.}

\item{annotation}{A \code{\link[GenomicRanges]{GRanges-class}} object with coordinates for the annotation.}

\item{windowsize}{Resolution in base-pairs for the curve upstream and downstream of the annotation.}

\item{insidewindows}{Number of data points for the curve inside the annotation.}

\item{range}{Distance upstream and downstream for which the enrichment profile is calculated.}

\item{category.column}{The name of a column in \code{data} that will be used for facetting of the plot.}

\item{plot}{Logical indicating whether a plot or the underlying data.frame is to be returned.}

\item{df.list}{A list() of data.frames, output from \code{plotEnrichment(..., plot=FALSE)}. If specified, option \code{data} will be ignored.}

\item{max.coverage.y}{Maximum coverage for positions on the y-axis.}

\item{min.coverage.x}{Minimum coverage for positions on the x-axis.}

\item{xmax}{Upper limit for the x-axis.}

\item{xbreaks.interval}{Interval for breaks on the x-axis.}

\item{cutoffs}{A vector with values that are plotted as horizontal lines. The names of the vector must match the context levels in \code{data$context}.}
}
\value{
A \code{\link[ggplot2]{ggplot}} object.
}
\description{
This page provides an overview of all \pkg{\link{methimpute}} plotting functions.
}
\section{Functions}{
\itemize{
\item \code{plotHistogram}: Plot a histogram of count values and fitted distributions.

\item \code{plotScatter}: Plot a scatter plot of read counts colored by methylation status.

\item \code{plotTransitionProbs}: Plot a heatmap of transition probabilities.

\item \code{plotConvergence}: Plot the convergence of the probability parameters.

\item \code{plotEnrichment}: Plot an enrichment profile around an annotation.

\item \code{plotPosteriorDistance}: Maximum posterior vs. distance to nearest covered cytosine.
}}

\examples{
## Get some toy data
file <- system.file("data","arabidopsis_toydata.RData",
                    package="methimpute")
data <- get(load(file))
print(data)
model <- callMethylation(data)
## Make nice plots
plotHistogram(model, total.counts=5)
plotScatter(model)
plotTransitionProbs(model)
plotConvergence(model)
plotPosteriorDistance(model$data)

## Get annotation data and make an enrichment profile
# Note that this looks a bit ugly because our toy data
# has only 200000 datapoints.
data(arabidopsis_genes)
plotEnrichment(model, annotation=arabidopsis_genes)

}
