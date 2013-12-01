unit Pizza;
interface
type
	IDeliverable = interface
	procedure Deliver;
	function GetDeliveryProgress: Double;
end;

type 
	TPizza = class(TInterfacedObject,IDeliverable)
	deliveryProgress: Double;
	isDelivering: Boolean;
	procedure Deliver;
	function GetDeliveryProgress: Double;
end;

implementation
	procedure TPizza.Deliver;
	begin
		WriteLn('Delivering');
		deliveryProgress := 0.0;
		isDelivering := true;
	end;

	function TPizza.GetDeliveryProgress: Double;
	begin
		result :=  deliveryProgress;
	end;
end.
