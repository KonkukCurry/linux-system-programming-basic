# finn_workstation

- linux system programming basic


{
 "cells": [
  {
   "cell_type": "code",
   "execution_count": 3,
   "id": "db1dd8fb",
   "metadata": {},
   "outputs": [],
   "source": [
    "import requests\n",
    "from bs4 import BeautifulSoup as bs\n",
    "import warnings\n",
    "warnings.filterwarnings('ignore')"
   ]
  },
  {
   "cell_type": "code",
   "execution_count": null,
   "id": "cbc8735d",
   "metadata": {},
   "outputs": [
    {
     "name": "stdout",
     "output_type": "stream",
     "text": [
      "what round do you want to know? ('q' to quit) : 1209\n",
      "what round do you want to know? ('q' to quit) : 1029\n",
      "#1029 Round Winning Number is : ['12', '30', '32', '37', '39', '41', '24']\n",
      "what round do you want to know? ('q' to quit) : 999\n",
      "#999 Round Winning Number is : ['1', '3', '9', '14', '18', '28', '34']\n"
     ]
    }
   ],
   "source": [
    "while 1:\n",
    "    try:\n",
    "        which_round_want_to_know = int(input(\"what round do you want to know? ('q' to quit) : \"))\n",
    "        if which_round_want_to_know == 'q':\n",
    "            break\n",
    "            \n",
    "        if which_round_want_to_know > 999:\n",
    "            s_which_round_want_to_know = f\"1%2C0{which_round_want_to_know-1000}\"\n",
    "            url = f'https://search.daum.net/search?w=tot&DA=LOT&rtmaxcoll=LOT&&q={s_which_round_want_to_know}%ED%9A%8C%EC%B0%A8%20%EB%A1%9C%EB%98%90'\n",
    "        else:\n",
    "            s_which_round_want_to_know = f\"{which_round_want_to_know}\"\n",
    "            url = f'https://search.daum.net/search?w=tot&DA=LOT&rtmaxcoll=LOT&&q={s_which_round_want_to_know}%ED%9A%8C%EC%B0%A8%20%EB%A1%9C%EB%98%90'\n",
    "\n",
    "        html = requests.get(url, verify = False)\n",
    "        # print(html)\n",
    "        # print(html.text)\n",
    "        soup = bs(html.text, 'html.parser')\n",
    "        # print(soup)\n",
    "\n",
    "        lot_num = soup.find_all(\"div\", class_ = 'lottonum')\n",
    "        #print(f\"{lot_num}\\n{len(lot_num)}개\\n\\n\")\n",
    "\n",
    "        round_num = soup.find_all(\"span\", class_ = 'f_red')\n",
    "        #print(f\"{round_num}\\n{len(round_num)}개\")\n",
    "        r_n = int(round_num[0].text.replace('회',''))\n",
    "        #print(r_n)\n",
    "        #print('\\n')\n",
    "\n",
    "        lot_num_list = []\n",
    "        for i in lot_num:\n",
    "            string = i.text\n",
    "        lot_num_list = string.split()\n",
    "        lot_num_list.remove('보너스')\n",
    "\n",
    "        print(f\"#{r_n} Round Winning Number is : {lot_num_list}\")\n",
    "    except:\n",
    "        continue\n",
    "print(\"------- Program Finished -------\")"
   ]
  }
 ],
 "metadata": {},
 "nbformat": 4,
 "nbformat_minor": 5
}
