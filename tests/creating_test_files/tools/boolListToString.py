def boolListToString(list):
  outputstring = ''
  for square in list:
    if square:
      outputstring+='1'
    else: outputstring+='0'
  return outputstring