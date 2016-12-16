SELECT COUNT(*) FROM 
(SELECT term FROM frequency WHERE docid = '10398_txt_earn' AND count = '1' UNION SELECT term FROM frequency WHERE docid='925_txt_trade' AND count='1');
