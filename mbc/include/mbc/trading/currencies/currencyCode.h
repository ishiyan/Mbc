#pragma once

namespace mbc::trading {

    //! Currency representations according to ISO 4217.
    //! This International Standard specifies the structure for a three-letter alphabetic code and an equivalent three-digit numeric code for the representation of currencies.
    //! The enumeration name is a currency name, the enumeration value is a numeric currency code.
    //! See http://en.wikipedia.org/wiki/ISO_4217.
    enum class CurrencyCode
    {
        // ReSharper disable IdentifierTypo
        // ReSharper disable CommentTypo

        // //! United Arab Emirates dirham.
        // Aed = 784,
        // //! Andorran franc (replaced with the Eur, 1999).
        // Adf = ?,
        // //! Andorran franc (replaced with the Eur, 1999).
        // Adp = 20,
        // //! Afghanistan afghani (replaced with Afn, 2003).
        // Afa = 4,
        // //! Afghanistan afghani.
        // Afn = 971,
        // //! Albanian lek (replaced with All).
        // Alk = ?,
        // //! Albanian lek.
        // All = 8,
        // //! Armenian dram.
        // Amd = 51,
        // //! Netherlands Antillian guilder.
        // Ang = 532,
        // //! Angolan kwanza.
        // Aoa = 973,
        // //! Angolan new kwanza (replaced with Aor, 1995).
        // Aon = 24,
        // //! Angolan kwanza readjustado (replaced with Aoa, 1999).
        // Aor = 982,
        // //! Argentine austral (replaced with Ars, 1991).
        // Ara = ?,
        // //! Argentine peso ley (replaced with Arp, 1983).
        // Arl = ?,
        // //! Argentine peso moneda nacional (replaced with Arl, 1970).
        // Arm = ?,
        // //! Peso argentino (replaced with Ara, 1985).
        // Arp = ?,

        //! Argentine peso.
        Ars = 32,

        // //! Austrian schilling (replaced with the Eur, 1999).
        // Ats = 40,

        //! Australian dollar.
        Aud = 36,

        // //! Aruban guilder.
        // Awg = 533,
        // //! Azerbaijani manat (replaced with Azn, 2006).
        // Azm = 31,
        // //! Azerbaijaniani manat.
        // Azn = 944,
        // //! Bosnia and Herzegovina convertible marks.
        // Bam = 977,
        // //! Barbados dollar.
        // Bbd = 52,
        // //! Bangladeshi taka.
        // Bdt = 50,
        // //! Belgian franc (convertible) (replaced with the Eur, 2002).
        // Bec = 993,
        // //! Belgian franc (replaced with the Eur, 1999).
        // Bef = 56,
        // //! Belgian franc(financial) (replaced with the Eur, 2002).
        // Bel = 992,
        // //! Bulgarian lev A/52 (replaced with Bgk, 1952).
        // Bgj = ?,
        // //! Bulgarian lev A/62 (replaced with Bgl, 1962).
        // Bgk = ?,
        // //! Bulgarian lev A/99 (replaced with Bgn, 1999).
        // Bgl = 100,
        // //! Bulgarian lev.
        // Bgn = 975,
        // //! Bahraini dinar.
        // Bhd = 48,
        // //! Burundian franc.
        // Bif = 108,
        // //! Bermuda dollar.
        // Bmd = 60,
        // //! Brunei dollar.
        // Bnd = 96,
        // //! Boliviano.
        // Bob = 68,
        // //! Bolivian peso (replaced with Bob, 1987).
        // Bop = ?,
        // //! Bolivian Mvdol (funds code).
        // Bov = 984,
        // //! Brazilian cruzeiro novo (replaced with Brc, 1986).
        // Brb = ?,
        // //! Brazilian cruzado (replaced with Brn, 1989).
        // Brc = ?,
        // //! Brazilian cruzeiro (replaced with Brr, 1993).
        // Bre = ?,

        //! Brazilian real.
        Brl = 986,

        // //! Brazilian cruzado novo (replaced with Bre, 1990).
        // Brn = ?,
        // //! Brazilian cruzeiro real (replaced with Brl, 1994).
        // Brr = ?,
        // //! Brazilian cruzeiro (replaced with Brb, 1967).
        // Brz = ?,
        // //! Bahamian dollar.
        // Bsd = 44,
        // //! Bhutan ngultrum.
        // Btn = 64,
        // //! Botswana pula.
        // Bwp = 72,
        // //! Belarussian ruble.
        // Byr = 974,
        // //! Belize dollar.
        // Bzd = 84,

        //! Canadian dollar.
        Cad = 124,

        // //! Franc Congolais.
        // Cdf = 976,
        // //! Swiss WIR Euro (complementary currency).
        // Che = 947,

        //! Swiss franc.
        Chf = 756,

        // //! Swiss WIR Franc (complementary currency).
        // Chw = 948,
        // //! Chilean escudo (replaced with Clp, 1975).
        // Cle = ?,
        // //! Chile Unidades de formento (funds code).
        // Clf = 990,

        //! Chilean peso.
        Clp = 152,
        //! Yuan renminbi.
        Cny = 156,

        // //! Chinese People's Bank dollar (replaced with Cny).
        // Cnx = ?,
        // //! Colombian peso.
        // Cop = 170,
        // //! Colombian Unidad de Valor Real.
        // Cou = 970,
        // //! Costa Rican colon.
        // Crc = 188,
        // //! Serbian dinar (replaced with Rsd, 2006).
        // Csd = 891,
        // //! Czechoslovak koruna A/53 (replaced with Csk, 1953).
        // Csj = ?,
        // //! Czechoslovak koruna (replaced with Czk and Skk, 1993).
        // Csk = 200,
        // //! Cuban peso.
        // Cup = 192,
        // //! Cape Verde escudo.
        // Cve = 132,
        // //! Cypriot pound (replaced with the Eur, 2008).
        // Cyp = 196,
        // //! Czech koruna.
        // Czk = 203,
        // //! East German Mark der DDR (replaced with Dem, 1990).
        // Ddm = 278,
        // //! German mark (replaced with the Eur, 1999).
        // Dem = 276,
        // //! Djibouti franc.
        // Djf = 262,

        //! Danish krone.
        Dkk = 208,

        // //! Dominican peso.
        // Dop = 214,
        // //! Algerian dinar.
        // Dzd = 12,
        // //! Ecuador sucre (replaced with Usd, 2000).
        // Ecs = 218,
        // //! Ecuador Unidad de Valor Constante (funds code, discontinued).
        // Ecv = 983,
        // //! Estonia kroon.
        // Eek = 233,
        // //! Egyptian pound.
        // Egp = 818,
        // //! Equatorial Guinean ekwele (replaced with Xaf).
        // Eqe = ?,
        // //! Eritrea nakfa.
        // Ern = 232,
        // //! Spanish peseta (account A)(replaced with Esp).
        // Esa = 996,
        // //! Spanish peseta (account B) (replaced with Esp).
        // Esb = 995,
        // //! Spanish peseta (replaced with the Eur, 1999).
        // Esp = 724,
        // //! Ethiopian birr.
        // Etb = 230,

        //! Euro.
        Eur = 978,

        // //! Finnish markka (replaced with the Eur, 1999).
        // Fim = 246,
        // //! Fiji dollar.
        // Fjd = 242,
        // //! Falkland Islands pound.
        // Fkp = 238,
        // //! French franc (replaced with the Eur, 1999).
        // Frf = 250,

        //! Pound sterling.
        Gbp = 826,
        //! Penny sterling.
        Gbx = 8260,

        // //! Georgian lari.
        // Gel = 981,
        // //! Ghanaian cedi (replaced with Ghs, 2007).
        // Ghc = 288,
        // //! Ghana cedi.
        // Ghs = 288,
        // //! Gibraltar pound.
        // Gip = 292,
        // //! Gambia dalasi.
        // Gmd = 270,
        // //! Guinean syli (replaced with Xof).
        // Gne = ?,
        // //! Guinea franc.
        // Gnf = 324,
        // //! Greek drachma (replaced with the Eur, 2001).
        // Grd = 300,
        // //! Guatemala quetzal.
        // Gtq = 320,
        // //! Guinea-Bissau peso (replaced with Xof).
        // Gwp = 624,
        // //! Guyana dollar.
        // Gyd = 328,

        //! Hong Kong dollar.
        Hkd = 344,

        // //! Honduras lempira.
        // Hnl = 340,
        // //! Croatian kuna.
        // Hrk = 191,
        // //! Haiti gourde.
        // Htg = 332,
        // //! Hungary forint.
        // Huf = 348,
        // //! Indonesia rupiah.
        // Idr = 360,
        // //! Irish punt (replaced with the Eur, 1999).
        // Iep = 372,
        // //! Israeli lira (replaced with Ilr, 1980).
        // Ilp = ?,
        // //! Israeli old sheqel (replaced with Ils, 1985).
        // Ilr = ?,

        //! Israeli new sheqel.
        Ils = 376,
        //! Indian rupee.
        Inr = 356,

        // //! Iraqi dinar.
        // Iqd = 368,
        // //! Iranian rial.
        // Irr = 364,
        // //! Icelandic old krona (replaced with Isk).
        // Isj = ?,

        //! Icelandic krona.
        Isk = 352,

        // //! Italian lira (replaced with the Eur, 1999).
        // Itl = 380,
        // //! Jamaican dollar.
        // Jmd = 388,
        // //! Jordanian dinar.
        // Jod = 400,

        //! Japanese yen.
        Jpy = 392,

        // //! Kenyan shilling.
        // Kes = 404,
        // //! Kyrgyzstan som.
        // Kgs = 417,
        // //! Cambodia riel.
        // Khr = 116,
        // //! Comoro franc.
        // Kmf = 174,
        // //! North Korean won.
        // Kpw = 408,

        //! South Korean won.
        Krw = 410,

        // //! Kuwaiti dinar.
        // Kwd = 414,
        // //! Cayman Islands dollar.
        // Kyd = 136,
        // //! Kazakhstan tenge.
        // Kzt = 398,
        // //! Laos kip (replaced with Lak).
        // Laj = ?,
        // //! Laos kip.
        // Lak = 418,
        // //! Lebanese pound.
        // Lbp = 422,
        // //! Sri Lanka rupee.
        // Lkr = 144,
        // //! Liberian dollar.
        // Lrd = 430,
        // //! Lesotho loti.
        // Lsl = 426,
        // //! Lithuanian litas.
        // Ltl = 440,
        // //! Luxembourg franc (replaced with the Eur, 1999).
        // Luf = 442,
        // //! Latvian lats.
        // Lvl = 428,
        // //! Libyan dinar.
        // Lyd = 434,
        // //! Moroccan dirham.
        // Mad = 504,
        // //! Monegasque franc (replaced with the Eur, 1999).
        // Mcf = ?,
        // //! Moldovan leu.
        // Mdl = 498,
        // //! Madagascar Malagasy ariary.
        // Mga = 969,
        // //! Madagascar Malagasy franc (replaced with Mga).
        // Mgf = ?,
        // //! Macedonia denar.
        // Mkd = 807,
        // //! Macedonia dinar A/93 (replaced with Mkd, 1993).
        // Mkn = ?,
        // //! Mali franc (replaced with Xof).
        // Mlf = ?,
        // //! Myanmar kyat.
        // Mmk = 104,
        // //! Mongolia tugrik.
        // Mnt = 496,
        // //! Macau pataca.
        // Mop = 446,
        // //! Mauritania ouguiya.
        // Mro = 478,
        // //! Maltese lira (replaced with the Eur, 2008).
        // Mtl = 470,
        // //! Mauritius rupee.
        // Mur = 480,
        // //! Maldive rupee (replaced with Mvr, 1981).
        // Mvq = ?,
        // //! Maldives rufiyaa.
        // Mvr = 462,
        // //! Malawi kwacha.
        // Mwk = 454,

        //! Mexican peso.
        Mxn = 484,

        // //! Mexican peso (replaced with Mxn).
        // Mxp = ?,
        // //! Mexican Unidad de Inversion (UDI) (funds code).
        // Mxv = 979,
        // //! Malaysian ringgit.
        // Myr = 458,
        // //! Mozambican metical (replaced with Mzn, 2006).
        // Mzm = 508,
        // //! Mozambique metical.
        // Mzn = 943,
        // //! Namibian dollar.
        // Nad = 516,
        // //! Newfoundland dollar (replaced with Cad, 1949).
        // Nfd = ?,
        // //! Nigeria naira.
        // Ngn = 566,
        // //! Nicaragua Cordoba oro.
        // Nio = 558,
        // //! Netherlands guilder (replaced with the Eur, 1999).
        // Nlg = 528,

        //! Norwegian krone.
        Nok = 578,

        // //! Nepalese rupee.
        //  Npr = 524,

        //! New Zealand dollar.
        Nzd = 554,

        // //! Oman Rial Omani.
        // Omr = 512,
        // //! Panama balboa.
        // Pab = 590,
        // //! Peruvian sol (replaced with Pei).
        // Peh = ?,
        // //! Peruvian inti (replaced with Pen).
        // Pei = ?,
        // //! Peruvian nuevo sol.
        // Pen = 604,
        // //! Papua New Guinea kina.
        // Pgk = 598,
        // //! Philippine peso.
        // Php = 608,
        // //! Pakistan rupee.
        // Pkr = 586,
        // //! Poland zloty.
        // Pln = 985,
        // //! Polish zloty A/94 (replaced with Pln, 1995).
        // Plz = 616,
        // //! Portuguese escudo (replaced with the Eur, 1999).
        // Pte = 620,
        // //! Paraguay guarani.
        // Pyg = 600,
        // //! Qatari rial.
        // Qar = 634,
        // //! Romanian leu A/05 (replaced with Ron, 2005).
        // Rol = 642,
        // //! Romanian new leu.
        // Ron = 946,
        // //! Serbian dinar.
        // Rsd = 941,

        //! Russian rouble.
        Rub = 643,

        // //! Russian rouble A/97 (replaced with Rub, 1997).
        // Rur = 810,
        // //! Rwanda franc.
        // Rwf = 646,
        // //! Saudi riyal.
        // Sar = 682,
        // //! Solomon Islands dollar.
        // Sbd = 90,
        // //! Seychelles rupee.
        // Scr = 690,
        // //! Sudanese dinar (replaced with Sdg, 2007).
        // Sdd = 736,
        // //! Sudanese pound.
        // Sdg = 938,

        //! Swedish krona.
        Sek = 752,
        //! Singapore dollar.
        Sgd = 702,

        // //! Saint Helena pound.
        // Shp = 645,
        // //! Slovenian tolar (replaced with the Eur, 2007).
        // Sit = 705,
        // //! Slovak koruna.
        // Skk = 703,
        // //! Sierra Leone leone.
        // Sll = 694,
        // //! San Marinese lira (replaced with the Eur, 1999).
        // Sml = ?,
        // //! Somali shilling.
        // Sos = 706,
        // //! Surinam dollar.
        // Srd = 968,
        // //! Suriname guilder (replaced with Srd, 2004).
        // Srg = 740,
        // //! São Tomé dobra.
        // Std = 678,
        // //! Soviet Union rouble (replaced with Rur, 1991).
        // Sur = ?,
        // //! Salvadoran colón (replaced with Usd).
        // Svc = 222,
        // //! Syrian pound.
        // Syp = 760,
        // //! Swaziland lilangeni.
        // Szl = 748,
        // //! Thailand baht.
        // Thb = 764,
        // //! Tajikistan rouble (replaced with Tjs, 2000).
        // Tjr = 762,
        // //! Tajikistan somoni.
        // Tjs = 972,
        // //! Turkmenistan manat.
        // Tmm = 795,
        // //! Tunisian dinar.
        // Tnd = 788,
        // //! Tonga pa'anga.
        // Top = 776,
        // //! Portuguese Timorese escudo (replaced with Idr).
        // Tpe = 626,
        // //! Turkish lira A/05 (replaced with Try, 2005).
        // Trl = 792,

        //! Turkish new lira.
        Try = 949,

        // //! Trinidad and Tobago dollar.
        // Ttd = 780,

        //! Taiwan new dollar.
        Twd = 901,

        // //! Tanzanian shilling.
        // Tzs = 834,
        // //! Ukrainian karbovanets (replaced with Uah).
        // Uak = 804,
        // //! Ukraine hryvnia.
        // Uah = 980,
        // //! Ugandan shilling A/87 (replaced with Ugx, 1987).
        // Ugs = ?,
        // //! Uganda shilling.
        // Ugx = 800,

        //! US dollar.
        Usd = 840,
        //! US cents.
        Usx = 8400,

        // //! US dollar (next day, funds code).
        // Usn = 997,
        // //! US dollar (same day, funds code).
        // Uss = 998,
        // //! Uruguay old peso (replaced with Uyu).
        // Uyn = ?,
        // //! Peso Uruguayo.
        // Uyu = 858,
        // //! Uzbekistan som.
        // Uzs = 860,
        // //! Vatican lira (replaced with the Eur, 1999).
        // Val = ?,
        // //! Venezuelan bolívar (replaced with Vef, 2008).
        // Veb = 862,
        // //! Venezuelan bolívar fuerte.
        // Vef = 937,
        // //! Vietnamese old đồng (replaced with Vnd).
        // Vnc = ?,
        // //! Vietnamese đồng.
        // Vnd = 704,
        // //! Vanuatu vatu.
        // Vuv = 548,
        // //! Samoan tala.
        // Wst = 882,
        // //! CFA franc BEAC.
        // Xaf = 950,

        //! Silver (one troy ounce).
        Xag = 961,
        //! Gold (one troy ounce).
        Xau = 959,

        // //! European composite unit (EURCO, bond market unit).
        // Xba = 955,
        // //! European monetary unit (EMU-6, bond market unit).
        // Xbb = 956,
        // //! European unit of account 9 (EUA-9, bond market unit).
        // Xbc = 957,
        // //! European unit of account 17 (EUA-17, bond market unit).
        // Xbd = 958,
        // //! East Caribbean dollar.
        // Xcd = 951,
        // //! International Monetary Fund special drawing rights.
        // Xdr = 960,
        // //! European currency unit (1 XEU = 1 Eur, replaced with the Eur, 1999).
        // Xeu = 954,
        // //! Bank for International Settlements gold franc (special settlement currency) (replaced with the Special Drawing Right (Xdr), 1st April 2003).
        // Xfo = 1,
        // //! International Union of Railways UIC franc (special settlement currency).
        // Xfu = 2,
        // //! CFA franc BCEAO.
        // Xof = 952,

        //! Palladium (one troy ounce).
        Xpd = 964,

        // //! CFP franc.
        // Xpf = 953,

        //! Platinum (one troy ounce).
        Xpt = 962,
        //! Code reserved for testing purposes.
        Xts = 963,
        //! No currency.
        Xxx = 0, //999,

        // //! South Yemeni dinar (replaced with Yer).
        // Ydd = 720,
        // //! Yemeni rial.
        // Yer = 886,
        // //! Yugoslav new dinar (replaced with Csd).
        // Yud = ?,
        // //! Yugoslav dinar (replaced with Csd).
        // Yum = 891,
        // //! South African financial rand (funds code, discontinued).
        // Zal = 991,

        //! South African rand.
        Zar = 710

        // //! Zambia kwacha.
        // Zmk = 894,
        // //! Zaïrean new zaïre (replaced with Cdf).
        // Zrn = 180,
        // //! Zaïrean zaïre (replaced with Zrn).
        // Zrz = ?,
        // //! Zimbabwe Rhodesian dollar (replaced with Zwd).
        // Zwc = ?,
        // //! Zimbabwe dollar.
        // Zwd = 716

        // ReSharper restore CommentTypo
        // ReSharper restore IdentifierTypo
    };
}
