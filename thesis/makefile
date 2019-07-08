define compile
	latex $(1)
	biber $(1)
	latex $(1)
	pdflatex $(1)
endef
all:
	vlna -l -v AaIiOoUuWwZz -f thesis.tex | sponge thesis.tex
	$(call compile,thesis)
	echo "\def\printable{}" | cat - thesis.tex > thesis-printable.tex
	$(call compile,thesis-printable)
	rm thesis-printable.tex
clean:
	git clean -x -f
