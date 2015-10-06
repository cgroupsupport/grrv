function [sbez,rbez]=bezou_z(abez,bbez,ret,pbez)

%solution minimale d'une équation de BEZOUT pour
%polynômes en z-1.
%
%		            -ret	
% 	abez * sbez + (q)     * bbez * rbez = pbez
%
%function [sbez,rbez]=bezou_z(abez,bbez,ret,pbez)

na=size(abez,2)-1;
nb=size(bbez,2)-1;
nadd=na+nb+ret-size(pbez,2);
Mbez=[abez';zeros(nb+ret-1,1)];


for i=1:(nb+ret-1)
	Madd=[zeros(i,1);abez';zeros(nb+ret-1-i,1)];
	Mbez=[Mbez Madd];
end
bbezret=[zeros(1,ret) bbez];
for i=0:(na-1)
	Madd=[zeros(i,1);bbezret';zeros(na-1-i,1)];
	Mbez=[Mbez Madd];
end
phi=[pbez zeros(1,nadd)];
result=inv(Mbez)*phi';
sbez=(result(1:nb+ret))';
rbez=(result(nb+ret+1:nb+ret+na));