/* */
do while lines('missing exports')
	a = linein('missing exports')
	parse var a '³ ' nr ' ³ ' name
	nr2 = nr + 0
	say '	_'name' @ 'nr2
end
